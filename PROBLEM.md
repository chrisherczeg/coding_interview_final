# Interview Problem Statement

## Context

You've joined a team developing firmware for an embedded system. The project includes a UART driver for serial communication. The previous developer left the team, and now the CI/CD pipeline is showing test failures.

## Your Mission

Debug and fix the failing tests in the UART driver codebase. This is a realistic scenario you might encounter in production firmware development.

## Instructions

1. **Build the project** using the provided build scripts or CMake
2. **Run the test suite** to identify failing tests
3. **Analyze the failures** - what pattern do you see?
4. **Find the bug** in the codebase
5. **Fix the bug** and verify all tests pass
6. **Be prepared to explain**:
   - What was the bug?
   - Why did it cause the observed failures?
   - What would happen in real hardware if this bug went to production?
   - How would you prevent this type of bug in the future?

## What We're Looking For

- **Debugging methodology**: How do you approach finding bugs in unfamiliar code?
- **Code comprehension**: Can you understand the hardware abstraction and FIFO implementation?
- **Problem-solving**: Can you identify the root cause, not just patch symptoms?
- **Embedded systems knowledge**: Do you understand the implications for real hardware?
- **Communication**: Can you clearly explain technical issues?

## Time Expectation

This should take approximately 30-45 minutes. Don't hesitate to:
- Ask questions about the code
- Add debug output
- Draw diagrams if helpful
- Explain your thought process as you work

## Notes

- The bug is realistic - it's based on real issues found in embedded systems
- All necessary code is provided - no external dependencies needed
- The tests are designed to help you find the bug
- You can modify the code however you need to debug the issue
- Focus on understanding WHY, not just making tests pass

Good luck!
