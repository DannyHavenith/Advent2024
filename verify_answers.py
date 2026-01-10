#!/usr/bin/env python3
"""
Verify that Advent of Code programs produce expected answers.

Reads expected-answers.txt and runs each program to verify its output.
"""

import re
import subprocess
import sys
from pathlib import Path


def parse_expected_answers(filepath: Path) -> dict[str, int]:
    """Parse expected-answers.txt into a dict mapping program name to expected answer."""
    answers = {}
    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) >= 2:
                program_name = parts[0]
                expected_answer = int(parts[1])
                answers[program_name] = expected_answer
    return answers


def get_program_path(program_name: str, base_dir: Path) -> Path:
    """Convert program name like '10a' to path like 'day10/day10a'."""
    # Extract day number and part (a/b)
    match = re.match(r'(\d+)([ab])', program_name)
    if not match:
        raise ValueError(f"Invalid program name format: {program_name}")
    
    day_num = match.group(1)
    part = match.group(2)
    
    day_dir = base_dir / f"day{day_num}"
    program_path = day_dir / f"day{day_num}{part}"
    
    return program_path


def extract_first_number(output: str) -> int | None:
    """Extract the first number from program output."""
    # Look for the first number in the output
    match = re.search(r'-?\d+', output)
    if match:
        return int(match.group(0))
    return None


def run_program(program_path: Path) -> tuple[bool, str, int | None]:
    """
    Run a program and extract its first number.
    
    Returns:
        (success, stdout, first_number)
    """
    if not program_path.exists():
        return False, f"Program not found: {program_path}", None
    
    try:
        result = subprocess.run(
            [str(program_path)],
            cwd=program_path.parent,
            capture_output=True,
            text=True,
            timeout=30
        )
        
        if result.returncode != 0:
            return False, f"Program exited with code {result.returncode}\n{result.stderr}", None
        
        first_number = extract_first_number(result.stdout)
        return True, result.stdout, first_number
        
    except subprocess.TimeoutExpired:
        return False, "Program timed out (30s)", None
    except Exception as e:
        return False, f"Error running program: {e}", None


def main():
    base_dir = Path(__file__).parent
    expected_answers_file = base_dir / "expected-answers.txt"
    
    if not expected_answers_file.exists():
        print(f"Error: {expected_answers_file} not found")
        sys.exit(1)
    
    expected_answers = parse_expected_answers(expected_answers_file)
    
    if not expected_answers:
        print("No expected answers found")
        sys.exit(1)
    
    print(f"Verifying {len(expected_answers)} program(s)...\n")
    
    passed = 0
    failed = 0
    
    for program_name in sorted(expected_answers.keys()):
        expected = expected_answers[program_name]
        program_path = get_program_path(program_name, base_dir)
        
        print(f"{program_name}: ", end="", flush=True)
        
        success, output, actual = run_program(program_path)
        
        if not success:
            print(f"❌ FAILED - {output}")
            failed += 1
            continue
        
        if actual is None:
            print(f"❌ FAILED - No number found in output")
            print(f"   Output: {output[:100]}")
            failed += 1
            continue
        
        if actual == expected:
            print(f"✓ PASSED (expected {expected})")
            passed += 1
        else:
            print(f"❌ FAILED - Expected {expected}, got {actual}")
            failed += 1
    
    print(f"\n{'='*60}")
    print(f"Results: {passed} passed, {failed} failed")
    print(f"{'='*60}")
    
    sys.exit(0 if failed == 0 else 1)


if __name__ == "__main__":
    main()
