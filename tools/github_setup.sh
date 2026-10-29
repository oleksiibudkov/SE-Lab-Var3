#!/usr/bin/env bash
# Creates the Scrum structure of the project on GitHub:
#   - labels: user story, task, MoSCoW priorities;
#   - sprints: 3-week milestones with a sprint goal;
#   - Product Backlog: user stories with acceptance criteria, story points and priority;
#   - Sprint Backlog: tasks of every story, linked as sub-issues;
#   - Scrum board: a GitHub Project with Status, Story Points and Priority.
# Run it once from Git Bash (Windows) or a terminal (Linux/macOS):
#     bash tools/github_setup.sh OWNER/REPO YOUR_LOGIN
# Needs GitHub CLI: https://cli.github.com  ->  gh auth login,  then  gh auth refresh -s project
# Safe to run again: labels, sprints, issues and the board are found by name and not created twice.
set -euo pipefail
REPO="${1:-}"; ME="${2:-}"
if [ -z "$REPO" ] || [ -z "$ME" ]; then
  echo "Usage: bash tools/github_setup.sh OWNER/REPO YOUR_LOGIN"; exit 1
fi
OWNER="${REPO%%/*}"
BOARD="SE-Lab-Var3 – Scrum board"
command -v gh >/dev/null || { echo "GitHub CLI (gh) is not installed: https://cli.github.com"; exit 1; }
gh auth status >/dev/null 2>&1 || { echo "Log in first:  gh auth login"; exit 1; }

# ---------------------------------------------------------------- helpers
label() {  # name color description
  gh label create "$1" --color "$2" --description "$3" --repo "$REPO" --force >/dev/null
}
sprint() {  # title due(YYYY-MM-DD) description -> milestone title (created once)
  local n
  n=$(gh api "repos/$REPO/milestones?state=all&per_page=100" --jq ".[] | select(.title==\"$1\") | .number")
  [ -n "$n" ] || gh api "repos/$REPO/milestones" -f title="$1" -f due_on="$2T21:00:00Z" -f description="$3" >/dev/null
  echo "$1"
}
close_sprint() {  # title
  local n
  n=$(gh api "repos/$REPO/milestones?state=all&per_page=100" --jq ".[] | select(.title==\"$1\") | .number")
  gh api -X PATCH "repos/$REPO/milestones/$n" -f state=closed >/dev/null
}
issue() {  # title body labels milestone -> number (created once, assigned to $ME)
  local n url args=()
  n=$(gh issue list --repo "$REPO" --state all --limit 500 --json number,title --jq ".[] | select(.title==\"$1\") | .number" | head -n 1)
  if [ -z "$n" ]; then
    args=(--repo "$REPO" --title "$1" --body "$2" --label "$3" --assignee "$ME")
    [ -n "$4" ] && args+=(--milestone "$4")
    url=$(gh issue create "${args[@]}")
    n=${url##*/}
  fi
  echo "$n"
}
node_id() { gh issue view "$1" --repo "$REPO" --json id --jq .id; }
sub_issue() {  # parent child: shows the task under its story
  gh api graphql -f query='mutation($p:ID!,$c:ID!){addSubIssue(input:{issueId:$p,subIssueId:$c}){issue{number}}}' \
    -f p="$(node_id "$1")" -f c="$(node_id "$2")" >/dev/null 2>&1 || true
}
done_issue() { gh issue close "$1" --repo "$REPO" --reason completed >/dev/null 2>&1 || true; }

STORIES=""   # "number:points:priority:status" for the board (status with _ instead of spaces)
TASKS=""     # "number:status"
LAST=""      # number of the story created last
story() {  # sprint id title role want benefit criteria(;-separated) points priority status -> LAST
  local box="- [ ]" crit="" body n
  [ "${10}" = Done ] && box="- [x]"
  IFS=';' read -ra items <<< "$7"
  for c in "${items[@]}"; do crit="$crit$box $c"$'\n'; done
  body=$(printf '**As** %s, **I want** %s, **so that** %s.\n\n### Acceptance criteria\n%s\n### Estimate\n| Story points | Priority |\n|---|---|\n| **%s** | **%s** have |\n\nThe tasks of this story are its sub-issues below.' \
    "$4" "$5" "$6" "$crit" "$8" "$9")
  n=$(issue "$2: $3" "$body" "user story,priority: $(echo "$9" | tr '[:upper:]' '[:lower:]')" "$1")
  [ "${10}" = Done ] && done_issue "$n"
  STORIES="$STORIES $n:$8:$9:${10// /_}"
  LAST=$n
}
task() {  # story sprint title status
  local n
  n=$(issue "$3" "Task of the user story #$1." "task" "$2")
  sub_issue "$1" "$n"
  [ "$4" = Done ] && done_issue "$n"
  TASKS="$TASKS $n:${4// /_}"
}

# ---------------------------------------------------------------- labels
echo "== Labels"
label "user story"       1D76DB "Product Backlog item: a need of a user, written as As/I want/so that"
label "task"             0E8A16 "Sprint Backlog item: technical work that implements a user story"
label "priority: must"   B60205 "MoSCoW: the product is not usable without it"
label "priority: should" D93F0B "MoSCoW: important, but the product works without it"
label "priority: could"  FBCA04 "MoSCoW: nice to have, done if time allows"

# ---------------------------------------------------------------- Sprint 1
echo "== Sprint 1"
S=$(sprint "Sprint 1 · 7 Sep – 25 Sep" 2026-09-25 \
  "Goal: the customer and the team agree on what the calculator does — the formulas, their limits and the user scenarios are analysed and modelled in UML. Velocity: 8 SP. Retro: modelling first saved time when coding.")
story "$S" "US-1" "Know the limits of the input data" "a customer" \
  "the formulas of Variant 3 to be analysed and their limits found" \
  "the program never shows values that are not defined" \
  "The domain of both branches is described: x = 0 is excluded, n > 3;Control values of y are calculated by hand for both branches" \
  3 Must Done; U=$LAST
task "$U" "$S" "Analyze the formulas and find the constraints (n > 3, x = 0)" Done
task "$U" "$S" "Calculate control examples by hand" Done
story "$S" "US-2" "Agree on the user scenarios" "a customer" \
  "UML models of the calculator" \
  "I can check the scenarios before the code is written" \
  "The Activity diagram shows the check of every input value;The Use Case diagram shows «include» and «extend»;The Lab 1 report is handed in" \
  5 Must Done; U=$LAST
task "$U" "$S" "Draw the Activity diagram with a check of every input value" Done
task "$U" "$S" "Draw the Use Case diagram with «include» and «extend»" Done
task "$U" "$S" "Prepare the report of Lab 1" Done
close_sprint "$S"

# ---------------------------------------------------------------- Sprint 2
echo "== Sprint 2"
S=$(sprint "Sprint 2 · 28 Sep – 16 Oct" 2026-10-16 \
  "Goal: a working console calculator — the user enters data from the keyboard or a file, sees the table of y(x) and can save it. Velocity: 18 SP. Retro: checking every value at once made the input friendly.")
story "$S" "US-3" "See the table of y(x)" "a user" \
  "to see y for every x from the start to the end value with a chosen step" \
  "I can study how the function changes" \
  "Both branches (x < 3 and x ≥ 3) give the hand-calculated control values;For x = 0 the table shows \"not defined\";Values that are too big are shown as \"too large\"" \
  5 Must Done; U=$LAST
task "$U" "$S" "Implement the calculation of y for both branches" Done
task "$U" "$S" "Print the result table with “not defined” and “too large”" Done
task "$U" "$S" "Compare the results with the control examples" Done
story "$S" "US-4" "Enter data from the keyboard" "a user" \
  "to type the input values and get a clear message about every wrong value" \
  "I can fix a mistake at once" \
  "Every value is checked right after it is typed;The message says what is wrong and gives an example;Text instead of a number does not break the program" \
  5 Must Done; U=$LAST
task "$U" "$S" "Implement keyboard input with a separate check of every value" Done
task "$U" "$S" "Write clear error messages with examples" Done
story "$S" "US-5" "Load data from a file" "a user" \
  "to load the input values from a file" \
  "I don't type them every time" \
  "The file holds 4 numbers: start end step n;A wrong or missing file does not change the current data" \
  3 Should Done; U=$LAST
task "$U" "$S" "Implement loading of data from a file" Done
task "$U" "$S" "Keep the previous data when the file is wrong" Done
story "$S" "US-6" "Save the table to a file" "a user" \
  "to save the result table to a file" \
  "I can use it later" \
  "After the table the program asks whether to save it;results.txt contains the data line and the same table" \
  2 Should Done; U=$LAST
task "$U" "$S" "Implement saving of the table to results.txt" Done
story "$S" "US-7" "Use the program without a manual" "a new user" \
  "a main menu and a help screen" \
  "I understand how to work with the program" \
  "The menu shows the formula, the current data and all options;Help explains every value, the file format and the special results" \
  3 Should Done; U=$LAST
task "$U" "$S" "Add the main menu and the Help screen" Done
task "$U" "$S" "Create the wireframe and the diagram of screen transitions" Done
task "$U" "$S" "Prepare the report of Lab 2" Done
close_sprint "$S"

# ---------------------------------------------------------------- Sprint 3 (current)
echo "== Sprint 3"
S=$(sprint "Sprint 3 · 19 Oct – 6 Nov" 2026-11-06 \
  "Goal: the project is transparent and easy to change — the work is planned and tracked on GitHub with Scrum, and the code is split into modules.")
story "$S" "US-8" "Track the work on GitHub with Scrum" "the team" \
  "the backlog, the sprints and the progress on GitHub" \
  "everyone sees what is done and what is next" \
  "The repository contains the code and the reports;The Product Backlog consists of user stories with acceptance criteria, story points and priority;Every sprint is a milestone with a goal, and the board shows the status of every item" \
  5 Must "In Progress"; U=$LAST
task "$U" "$S" "Create a GitHub account and a new repository" Done
task "$U" "$S" "Set up access to the repository (collaborators)" Done
task "$U" "$S" "Push the source code and reports to the repository" Done
task "$U" "$S" "Create the Product Backlog: user stories, labels and sprints" Done
task "$U" "$S" "Set up the Scrum board" Done
task "$U" "$S" "Prepare the report of Lab 3" "In Progress"
story "$S" "US-9" "Split the program into modules" "a developer" \
  "the program split into modules" \
  "each part can be read, changed and tested separately" \
  "The calculation, the checks, the input/output and the screens are separate modules with headers;The program behaves exactly as before: the same output for the same input;It builds with make and with CMake (Visual Studio) without warnings" \
  5 Should "In Progress"; U=$LAST
task "$U" "$S" "Move the calculation and the checks into the core module" "In Progress"
task "$U" "$S" "Move console input, file loading and table printing into the io module" "In Progress"
task "$U" "$S" "Move the menu, result and help screens into the ui module" "In Progress"
task "$U" "$S" "Add build files: Makefile and CMakeLists.txt" "In Progress"
task "$U" "$S" "Check that the output is the same as before the split" "In Progress"

# ---------------------------------------------------------------- Sprint 4 (forecast)
echo "== Sprint 4"
S=$(sprint "Sprint 4 · 9 Nov – 27 Nov" 2026-11-27 \
  "Goal: the product is described precisely — a Software Requirements Specification that the customer can accept.")
story "$S" "US-10" "Agree on the requirements" "a customer" \
  "a Software Requirements Specification of the calculator" \
  "I can accept the product against clear requirements" \
  "The SRS follows a chosen template (for example IEEE 830);It has the purpose, definitions, overall description, functional and non-functional requirements;Every requirement can be checked" \
  8 Must "Product Backlog"; U=$LAST
task "$U" "$S" "Choose a template for the SRS" "Product Backlog"
task "$U" "$S" "Write the purpose, definitions and system overview" "Product Backlog"
task "$U" "$S" "Write the overall description of the program" "Product Backlog"
task "$U" "$S" "Write the functional and non-functional requirements" "Product Backlog"
task "$U" "$S" "Prepare the report of Lab 4" "Product Backlog"

# ---------------------------------------------------------------- Sprint 5 (forecast)
echo "== Sprint 5"
S=$(sprint "Sprint 5 · 30 Nov – 18 Dec" 2026-12-18 \
  "Goal: the calculator is reliable — wrong input and file problems never crash it, and unit tests protect the calculation.")
story "$S" "US-11" "Never crash on bad input" "a user" \
  "the program to handle any wrong input or file problem" \
  "it never stops unexpectedly" \
  "Wrong values and file errors are reported with a message, not a crash;Errors are handled with exceptions in one place" \
  5 Must "Product Backlog"; U=$LAST
task "$U" "$S" "Add exception handling for wrong input and files" "Product Backlog"
task "$U" "$S" "Show one clear message for every type of error" "Product Backlog"
story "$S" "US-12" "Protect the results with unit tests" "a developer" \
  "unit tests for the calculation and the input checks" \
  "a change cannot silently break the results" \
  "Tests cover both branches of y(x), x = 0 and a big n;Tests cover every input check;All tests pass before a merge into main" \
  8 Must "Product Backlog"; U=$LAST
task "$U" "$S" "Set up a unit test framework" "Product Backlog"
task "$U" "$S" "Write unit tests for the calculation of y" "Product Backlog"
task "$U" "$S" "Write unit tests for the input checks" "Product Backlog"
task "$U" "$S" "Prepare the report of Lab 5" "Product Backlog"

# ---------------------------------------------------------------- not planned yet
echo "== Product Backlog"
story "" "US-13" "Choose the precision of the table" "a user" \
  "to choose how many digits the table shows" \
  "the results are easier to read" \
  "The precision from 1 to 10 digits is set in the menu;It is used both on the screen and in results.txt" \
  2 Could "Product Backlog"
story "" "US-14" "Export the table to CSV" "a user" \
  "to save the table as a CSV file" \
  "I can open it in Excel" \
  "The file has the header x,y and one row for every x;\"not defined\" and \"too large\" are kept as text" \
  3 Could "Product Backlog"

# ---------------------------------------------------------------- Scrum board
echo "== Scrum board"
P=$(gh project list --owner "$OWNER" --format json --jq ".projects[] | select(.title==\"$BOARD\") | .number")
if [ -z "$P" ]; then
  P=$(gh project create --owner "$OWNER" --title "$BOARD" --format json --jq .number)
fi
gh project link "$P" --owner "$OWNER" --repo "$REPO" >/dev/null 2>&1 || true
gh project edit "$P" --owner "$OWNER" --visibility PUBLIC \
  --description "Scrum board of the Function Table Calculator (Variant 3)" \
  --readme "Columns: Product Backlog → Sprint Backlog → In Progress → Done. Sprints are milestones, user stories carry story points and a MoSCoW priority, tasks are sub-issues of their story." >/dev/null
PID=$(gh project view "$P" --owner "$OWNER" --format json --jq .id)
fid() { gh project field-list "$P" --owner "$OWNER" --format json --jq ".fields[] | select(.name==\"$1\") | .id"; }
[ -n "$(fid "Story Points")" ] || gh project field-create "$P" --owner "$OWNER" --name "Story Points" --data-type NUMBER >/dev/null
[ -n "$(fid "Priority")" ] || gh project field-create "$P" --owner "$OWNER" --name "Priority" --data-type SINGLE_SELECT \
  --single-select-options "Must,Should,Could" >/dev/null
STATUS=$(fid "Status")
# Scrum columns instead of the default Todo / In Progress / Done
gh api graphql -f query='mutation($f:ID!){updateProjectV2Field(input:{fieldId:$f,singleSelectOptions:[
  {name:"Product Backlog",color:GRAY,description:"Ordered list of everything the product needs"},
  {name:"Sprint Backlog",color:BLUE,description:"Chosen for the current sprint, not started"},
  {name:"In Progress",color:YELLOW,description:"Being worked on in this sprint"},
  {name:"Done",color:GREEN,description:"Meets the Definition of Done"}]}){projectV2Field{... on ProjectV2SingleSelectField{id}}}}' \
  -f f="$STATUS" >/dev/null
OPTIONS=$(gh project field-list "$P" --owner "$OWNER" --format json \
  --jq '.fields[] | select(.options) | .name as $f | .options[] | "\($f):\(.name)=\(.id)"')
opt() {  # field option -> option id
  echo "$OPTIONS" | grep -F "$1:$2=" | head -n 1 | cut -d= -f2
}
SP=$(fid "Story Points"); PR=$(fid "Priority")
add() {  # issue status [points priority]
  local item
  item=$(gh project item-add "$P" --owner "$OWNER" --url "https://github.com/$REPO/issues/$1" --format json --jq .id)
  gh project item-edit --id "$item" --project-id "$PID" --field-id "$STATUS" --single-select-option-id "$(opt Status "$2")" >/dev/null
  if [ -n "${3:-}" ]; then
    gh project item-edit --id "$item" --project-id "$PID" --field-id "$SP" --number "$3" >/dev/null
    gh project item-edit --id "$item" --project-id "$PID" --field-id "$PR" --single-select-option-id "$(opt Priority "$4")" >/dev/null
  fi
}
for s in $STORIES; do
  IFS=: read -r n pts pri st <<< "$s"; add "$n" "${st//_/ }" "$pts" "$pri"
done
for t in $TASKS; do
  IFS=: read -r n st <<< "$t"; add "$n" "${st//_/ }"
done

echo "Done. Backlog: https://github.com/$REPO/issues   Sprints: https://github.com/$REPO/milestones"
echo "Board: https://github.com/users/$OWNER/projects/$P"
