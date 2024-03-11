import re
import sys
import subprocess
from datetime import date

def update_hand_in_date(file_path):
	file_path = f"{file_path}.c"

    # Get today's date
	today = date.today().strftime("%Y-%m-%d")
    
    # Open the file
	with open(file_path, 'r') as file:
		content = file.read()

    # Use regular expression to find and replace the hand in date
	updated_content = re.sub(r'(Hand in date:\s*)(\d{4}-\d{2}-\d{2})', f'Hand in date: {today}', content)

    # Write back the updated content to the file
	with open(file_path, 'w') as file:
		file.write(updated_content)

	print("Hand in date updated successfully.")


def compile_file(file_name):
    compile_command = f"gcc -std=c99 -Wall -Werror -g -I lib -o {file_name}test test_imp.c {file_name}.c lib/dlist.c"

    if file_name == "graph":
        compile_command += " lib/array_1d.c lib/array_2d.c"
    elif file_name == "graph2":
         compile_command += ""

    try:
        subprocess.run(compile_command, shell=True, check=True)
        print(f"Compiled {file_name} sucessfully..")
    except subprocess.CalledProcessError as e:
        print(f"Error compiling {file_name}: {e}")
        return 1
 
    return 0


def mem_leak_test(file_name): 
    mem_test_command = f"valgrind --leak-check=full --show-reachable=yes ./{file_name}test 10"

    with open('/dev/null', 'w') as devnull:
        try:
            subprocess.run(mem_test_command, shell=True, stdout=devnull, stderr=subprocess.STDOUT, check=True)
            print(f"No memory-leaks in {file_name}.")
        except subprocess.CalledProcessError:
            print(f"Memory-leaks in {file_name}.")
            return 1

    return 0


# Specify the file path here
files = ["graph", "graph2", "is_connected"]
comp_err = 0
update_hand_in_date(files[0])
update_hand_in_date(files[1])
update_hand_in_date(files[2])

compile_file(files[0])
compile_file(files[1])

if comp_err > 0:
    quit()
    
if "--no-mem" not in sys.argv:
	mem_leak_test(files[0])
	mem_leak_test(files[1])
