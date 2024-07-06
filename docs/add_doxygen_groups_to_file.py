# https://chatgpt.com/c/518a38d9-f0d8-469b-9ddc-d570809709fe
import os

# Configuration
group_name = "LuaSrc"
group_description = "Lua 5.4.6 Sources"
file_extensions = [".c", ".h"]  # Process both .c and .h files

# Doxygen group template
group_start = (
    "/** @defgroup " + group_name + " " + group_description + " */\n"
    "/** @{ */\n"
)
group_end = "/** @} */\n"


def find_last_include_or_define(content):
    """
    Find the last line of include or define statements in the file.
    """
    last_index = 0
    for i, line in enumerate(content):
        if line.strip().startswith(("#include", "#define")):
            last_index = i + 1  # Include the line after the last include/define
    return last_index

def add_doxygen_groups_to_file(file_path):
    with open(file_path, 'r') as file:
        content = file.readlines()

    # Check if the file already contains Doxygen group annotations
    if any('@defgroup' in line for line in content):
        print(f"Skipping {file_path}: Already contains Doxygen group annotations.")
        return

    # Find the last line of include or define statements
    last_index = find_last_include_or_define(content)

    # Insert group annotations after the last include or define
    content.insert(last_index, group_start)
    content.append(group_end)

    with open(file_path, 'w') as file:
        file.writelines(content)

    print(f"Updated {file_path}")

    
def process_directory(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in file_extensions):
                file_path = os.path.join(root, file)
                add_doxygen_groups_to_file(file_path)

# Run the script on the current directory
if __name__ == "__main__":
    project_directory = "/home/yair/alyn/project/game/luaEffects/lua-5.4.6"
    process_directory(project_directory)

