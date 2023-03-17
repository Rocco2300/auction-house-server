import re


def parse_header(file_path):
    file = open(file_path, "r")

    lines = []
    for line in file:
        lines.append(line)

    file.close()
    return lines


def find_fields(lines):
    skip = True
    fields = []
    for i in range(len(lines)):
        line = lines[i]
        normalized_line = ' '.join(line.split())

        field_regex = re.compile(r'(?!return\b)\b[A-Za-z_:]+\s\w+{?}?;+$')
        field = field_regex.search(normalized_line)

        if field is not None:
            fields.append(field.group(0))

    return fields


def get_var_name(field):
    tokens = re.split(r'[ {};]', field)
    tokens.remove('')

    return tokens[1]


def get_var_names(fields):
    ret = []
    for field in fields:
        ret.append(get_var_name(field))

    return ret


def undecorated(var_name):
    name_tokens = re.split('(m_|_)', var_name)
    name_tokens.sort()
    undecorated_var_name = name_tokens[-1]

    return undecorated_var_name


def remove_current_metadata(lines):
    to_remove = []
    deleting = False
    for index in range(len(lines)):
        line = lines[index]

        if line.find('#pragma FIELDS') != -1:
            next_line = lines[index+1]
            if next_line.find('std::unordered_map') != -1:
                deleting = True
                continue

        if line.find('};') != -1 and deleting:
            to_remove.append(index)
            break

        if deleting:
            to_remove.append(index)

    while len(to_remove) > 0:
        index = to_remove.pop()
        del lines[index]


def find_insert_index(lines):
    insert_at = 0
    for index in range(len(lines)):
        line = lines[index]
        if line.find('#pragma FIELDS') != -1:
            insert_at = index+1
            break
    return insert_at


def build_metadata(var_name_list):
    fields_map = '\tstd::unordered_map<std::string, Field> fields = {\n'
    for var_name in var_name_list:
        fields_map += '\t\t\t{"' + undecorated(var_name) + '", ' + var_name + '},\n'
    fields_map += '\t};\n'
    return fields_map


def write_to_file(file_path, file_data):
    file = open(file_path, 'w')

    file.write(''.join(file_data))
    file.truncate()


if __name__ == '__main__':
    lines = parse_header('C:/Users/grigo/Repos/auction-house/server/include/User.h')
    remove_current_metadata(lines)
    fields = find_fields(lines)
    var_names = get_var_names(fields)

    index_to_insert = find_insert_index(lines)
    metadata = build_metadata(var_names)
    lines.insert(index_to_insert, metadata)

    write_to_file('C:/Users/grigo/Repos/auction-house/server/include/User.h', lines)



