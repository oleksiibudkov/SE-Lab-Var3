#!/usr/bin/env bash
# Creates labels, milestones, epic tasks and subtasks of the project on GitHub.
# Run it once from Git Bash (Windows) or a terminal (Linux/macOS):
#     bash tools/github_setup.sh OWNER/REPO YOUR_LOGIN [TEAMMATE_LOGIN]
# Needs GitHub CLI: https://cli.github.com  ->  first run:  gh auth login
# Safe to run again: things that already exist (same title) are not created twice.
set -e
REPO="$1"; ME="$2"; TEAM="${3:-$2}"
if [ -z "$REPO" ] || [ -z "$ME" ]; then
  echo "Usage: bash tools/github_setup.sh OWNER/REPO YOUR_LOGIN [TEAMMATE_LOGIN]"; exit 1
fi
command -v gh >/dev/null || { echo "GitHub CLI (gh) is not installed: https://cli.github.com"; exit 1; }
gh auth status >/dev/null 2>&1 || { echo "Log in first:  gh auth login"; exit 1; }

echo "== Labels"
gh label create "Epic"    --color 5319E7 --description "Big task of one milestone" --repo "$REPO" --force
gh label create "SubTask" --color 0E8A16 --description "Part of an epic task"      --repo "$REPO" --force

milestone() {  # title due description  -> number
  local n
  n=$(gh api "repos/$REPO/milestones?state=all&per_page=100" --jq ".[] | select(.title==\"$1\") | .number")
  [ -n "$n" ] || n=$(gh api "repos/$REPO/milestones" -f title="$1" -f due_on="$2T21:00:00Z" -f description="$3" --jq .number)
  echo "$n"
}
issue() {  # title body label milestone assignee  -> number
  local n url
  n=$(gh issue list --repo "$REPO" --state all --limit 500 --json number,title --jq ".[] | select(.title==\"$1\") | .number" | head -n 1)
  if [ -z "$n" ]; then
    url=$(gh issue create --repo "$REPO" --title "$1" --body "$2" --label "$3" --milestone "$4" --assignee "$5" 2>/dev/null) ||
    url=$(gh issue create --repo "$REPO" --title "$1" --body "$2" --label "$3" --milestone "$4")
    n=${url##*/}
  fi
  echo "$n"
}
done_issue() { gh issue close "$1" --repo "$REPO" --reason completed >/dev/null 2>&1 || true; }


echo "== Iteration 1 – Domain modelling (Lab 1)"
M='Iteration 1 – Domain modelling (Lab 1)'
MS=$(milestone "$M" 2026-09-25 'Analysis of the individual task and UML models of the domain.')
E=$(issue '[Epic] Model the domain of Variant 3 with UML' 'We should analyze the system of formulas of Variant 3, find its constraints and describe the algorithm and the functional requirements of the program with UML diagrams.' Epic "$M" "$ME")
LIST=""
S=$(issue 'Analyze the formulas and find the constraints (n > 3, x = 0)' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Calculate control examples by hand' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Draw the Activity diagram with a check of every input value' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Draw the Use Case diagram with «include» and «extend»' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Prepare the report of Lab 1' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
gh issue edit "$E" --repo "$REPO" --body "$(printf '%s\n\n**Subtasks:**\n%s' 'We should analyze the system of formulas of Variant 3, find its constraints and describe the algorithm and the functional requirements of the program with UML diagrams.' "$LIST")" >/dev/null
done_issue "$E"
gh api -X PATCH "repos/$REPO/milestones/$MS" -f state=closed >/dev/null

echo "== Iteration 2 – Coding of domain (Lab 2)"
M='Iteration 2 – Coding of domain (Lab 2)'
MS=$(milestone "$M" 2026-10-16 'C++ console program for the formulas of Variant 3 and its user interface.')
E=$(issue '[Epic] Implement the function table calculator in C++' 'We should write a C++ console program that calculates y for every x of the range with the given step, checks the input data and has a convenient user interface.' Epic "$M" "$ME")
LIST=""
S=$(issue 'Implement the calculation of y for both branches' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Implement keyboard input with a separate check of every value' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Implement loading of data from a file' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Implement the result table and saving to results.txt' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Add the main menu and the Help screen' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Create the wireframe and the diagram of screen transitions' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Prepare the report of Lab 2' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
gh issue edit "$E" --repo "$REPO" --body "$(printf '%s\n\n**Subtasks:**\n%s' 'We should write a C++ console program that calculates y for every x of the range with the given step, checks the input data and has a convenient user interface.' "$LIST")" >/dev/null
done_issue "$E"
gh api -X PATCH "repos/$REPO/milestones/$MS" -f state=closed >/dev/null

echo "== Iteration 3 – Planning of system activities (Lab 3)"
M='Iteration 3 – Planning of system activities (Lab 3)'
MS=$(milestone "$M" 2026-10-30 'Project tracking on GitHub: repository, team, milestones, issues and git.')
E=$(issue '[Epic] Set up project tracking on GitHub' 'We should create a repository for the project, add the team, plan the work of all labs as milestones, epics and subtasks, and store the code and reports in git.' Epic "$M" "$ME")
LIST=""
S=$(issue 'Create a GitHub account and a new repository' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Add a collaborator to the repository' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Create milestones and labels Epic and SubTask' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Create epics and subtasks for all labs and assign them' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Push the source code and reports to the repository' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [x] #$S"$'\n'
done_issue "$S"
S=$(issue 'Prepare the report of Lab 3' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
gh issue edit "$E" --repo "$REPO" --body "$(printf '%s\n\n**Subtasks:**\n%s' 'We should create a repository for the project, add the team, plan the work of all labs as milestones, epics and subtasks, and store the code and reports in git.' "$LIST")" >/dev/null

echo "== Iteration 4 – Software requirements specification (Lab 4)"
M='Iteration 4 – Software requirements specification (Lab 4)'
MS=$(milestone "$M" 2026-11-20 'Software Requirements Specification of the program.')
E=$(issue '[Epic] Write the SRS of the program' 'We should choose a pattern of the Software Requirements Specification and fill it in according to the results of the previous labs.' Epic "$M" "$ME")
LIST=""
S=$(issue 'Choose a pattern for the SRS' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Write the purpose, definitions and system overview' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Write the overall description of the program' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Write the functional and non-functional requirements' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Update the repository and the issues on GitHub (Lab 4)' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Prepare the report of Lab 4' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
gh issue edit "$E" --repo "$REPO" --body "$(printf '%s\n\n**Subtasks:**\n%s' 'We should choose a pattern of the Software Requirements Specification and fill it in according to the results of the previous labs.' "$LIST")" >/dev/null

echo "== Iteration 5 – Testing of code (Lab 5)"
M='Iteration 5 – Testing of code (Lab 5)'
MS=$(milestone "$M" 2026-12-11 'Functions, exception handling and unit tests of the program.')
E=$(issue '[Epic] Refactor the program and cover it with unit tests' 'We should improve the structure of the code with functions and exception handling and check the program with unit tests.' Epic "$M" "$ME")
LIST=""
S=$(issue 'Move the calculation and the checks into separate functions' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Add exception handling for wrong input and files' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Write unit tests for the calculation of y and the input checks' "Part of the epic task #$E." SubTask "$M" "$TEAM")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Update the repository and the issues on GitHub (Lab 5)' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
S=$(issue 'Prepare the report of Lab 5' "Part of the epic task #$E." SubTask "$M" "$ME")
LIST="$LIST- [ ] #$S"$'\n'
gh issue edit "$E" --repo "$REPO" --body "$(printf '%s\n\n**Subtasks:**\n%s' 'We should improve the structure of the code with functions and exception handling and check the program with unit tests.' "$LIST")" >/dev/null

echo "Done. Open https://github.com/$REPO/milestones"
