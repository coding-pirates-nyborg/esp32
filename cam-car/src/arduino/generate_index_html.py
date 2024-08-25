import os

# Set the current directory to the script's directory
os.chdir(os.path.dirname(os.path.abspath(__file__)))

# Rest of your code...

# convert_html_to_cpp.py
def convert_html_to_cpp(input_file, output_file):
    ''' 
    Convert the input html file to a C++ string literal and write it to the output file.
    '''
    with open(input_file, 'r') as f:
        html_content = f.read()

    cpp_literal = 'static const char PROGMEM INDEX_HTML[] = R"rawliteral(\n' + html_content + '\n)rawliteral";\n'

    with open(output_file, 'w') as f:
        f.write(cpp_literal)

if __name__ == "__main__":
    print('Converting index.html to index_html.h')
    convert_html_to_cpp('index.html', 'index_html.h')