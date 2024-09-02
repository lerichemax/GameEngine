import os
import argparse

def parse_arguments():
    parser = argparse.ArgumentParser(description="Generate C++ class files")
    parser.add_argument("class_name", help="Name of the derived class")
    parser.add_argument("base_class", help="Name of the base class")
    parser.add_argument("base_class_header", help="Header file of the base class")
    return parser.parse_args()


def create_class_files(class_name, base_class, base_class_header):
    # Read the header template
    with open('class_template.h', 'r') as file:
        header_content = file.read()

    # Replace placeholders in the header template
    header_content = header_content.replace('{{CLASS_NAME}}', class_name)
    header_content = header_content.replace('{{BASE_CLASS}}', base_class)
    header_content = header_content.replace('{{BASE_CLASS_HEADER}}', base_class_header)

    # Read the cpp template
    with open('class_template.cpp', 'r') as file:
        cpp_content = file.read()

    # Replace placeholders in the cpp template
    cpp_content = cpp_content.replace('{{CLASS_NAME}}', class_name)

    # Write the header file
    header_file_path = os.path.join(os.getcwd(), f'{class_name}.h')
    with open(header_file_path, 'w') as file:
        file.write(header_content)

    # Write the cpp file
    cpp_file_path = os.path.join(os.getcwd(), f'{class_name}.cpp')
    with open(cpp_file_path, 'w') as file:
        file.write(cpp_content)

    print(f"Generated {header_file_path} and {cpp_file_path}")

# Example usage
if __name__ == "__main__":

    args = parse_arguments()

    create_class_files(args.class_name, args.base_class, args.base_class_header)