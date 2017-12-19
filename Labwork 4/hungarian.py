def print_matrix(matrix):
    for row in matrix:
        for elem in row:
            print("% 5s " % str(elem), end="")
        print()
    print()


# Step 0: initial


def convert_to_string(lst):
    res = []
    for elem in lst:
        res.append(str(elem))
    return res


# Steps 1 & 2: subtraction of least values from rows and columns


def convert_matrix_to_string(matrix):
    res = []
    for row in matrix:
        res.append(convert_to_string(row))
    return res


def convert_to_int(lst):
    res = []
    for elem in lst:
        res.append(int(elem))
    return res


def fin_min_in_row_or_column(lst):
    return min(convert_to_int(lst))


def get_column(matrix, j):
    column = []
    for i in range(0, len(matrix)):
        column.append(matrix[i][j])
    return column


def subtract_from_rows(matrix):
    for i in range(0, len(matrix)):
        min_elem = fin_min_in_row_or_column(matrix[i])
        for j in range(0, len(matrix[i])):
            matrix[i][j] = str(int(matrix[i][j]) - min_elem)
    return matrix


def subtract_from_columns(matrix):
    for j in range(0, len(matrix[0])):
        min_elem = fin_min_in_row_or_column(get_column(matrix, j))
        for i in range(0, len(matrix)):
            matrix[i][j] = str(int(matrix[i][j]) - min_elem)
    return matrix


# Steps 3 & 4: marking one zero in each row and column and cross out others


def cycled_inc(k, dim):
    if k == dim - 1:
        return 0
    return k + 1


def contains_one_zero(lst, marked=False):
    res = -1
    for i in range(0, len(lst)):
        if (lst[i] == "0" or marked and lst[i] == "[0]") and res == -1:
            res = i
        elif lst[i] == "0" or marked and lst[i] == "[0]":
            return -1
    return res


def cross_zeros_in_column(matrix, j, i_of_zero):
    for i in range(0, len(matrix)):
        if matrix[i][j] == "0" and i != i_of_zero:
            matrix[i][j] = "x"
    return matrix


def mark_zeros_in_rows(matrix):
    row_dim = len(matrix)
    need_to_search = row_dim - 1
    i = 0
    while True:
        j_of_zero = contains_one_zero(matrix[i])
        if j_of_zero != -1:
            matrix[i][j_of_zero] = "[0]"
            matrix = cross_zeros_in_column(matrix, j_of_zero, i)
            i = cycled_inc(i, row_dim)
            need_to_search += 1
        elif need_to_search == 0:
            break
        else:
            need_to_search -= 1
            i = cycled_inc(i, row_dim)
    return matrix


#  I know about DRY, but let's not complicate things now


def cross_zeros_in_row(matrix, i, j_of_zero):
    for j in range(0, len(matrix[0])):
        if matrix[i][j] == "0" and j != j_of_zero:
            matrix[i][j] = "x"
    return matrix


def mark_zeros_in_columns(matrix):
    col_dim = len(matrix[0])
    need_to_search = col_dim - 1
    j = 0
    while True:
        print(get_column(matrix, j))
        i_of_zero = contains_one_zero(get_column(matrix, j))
        if i_of_zero != -1:
            matrix[i_of_zero][j] = "[0]"
            matrix = cross_zeros_in_row(matrix, i_of_zero, j)
            j = cycled_inc(j, col_dim)
            need_to_search += 1
        elif need_to_search == 0:
            break
        else:
            need_to_search -= 1
            j = cycled_inc(j, col_dim)
    return matrix


# Step 5: mark one unmarked zero, if there are any, and cross out neighbouring ones


def mark_any_zero(matrix):
    for i in range(0, len(matrix)):
        for j in range(0, len(matrix[0])):
            if matrix[i][j] == "0":
                matrix[i][j] = "[0]"
                matrix = cross_zeros_in_column(matrix, j, i)
                matrix = cross_zeros_in_row(matrix, i, j)
    return matrix


# Steps 6 & 7: stroke lines and rows, containing marked and crossed zeros


def has_marked_and_crossed(lst):
    has_marked = False
    has_crossed = False
    for elem in lst:
        if elem == "[0]":
            has_marked = True
        elif elem == "x":
            has_crossed = True
    return has_marked and has_crossed


def is_stroked(elem):
    return elem[0] == "{" and elem[-1] == "}"


def stroke(elem):
    return "{" + elem + "}"


def stroke_rows(matrix):
    for i in range(0, len(matrix)):
        if has_marked_and_crossed(matrix[i]):
            for j in range(0, len(matrix[i])):
                if not is_stroked(matrix[i][j]):
                    matrix[i][j] = stroke(matrix[i][j])
    return matrix


def stroke_columns(matrix):
    for j in range(0, len(matrix[0])):
        if has_marked_and_crossed(get_column(matrix, j)):
            for i in range(0, len(matrix)):
                if not is_stroked(matrix[i][j]):
                    matrix[i][j] = stroke(matrix[i][j])
    return matrix


# Step 8: subtract min value from not stroked numbers, add it to intersections of strokes


def find_min_in_not_stroked(matrix):
    res = float('inf')
    for row in matrix:
        for elem in row:
            if elem not in ["x", "[0]"] and not is_stroked(elem) and int(elem) < res:
                res = int(elem)
    return res


def is_row_or_column_stroked(lst):
    res = True
    for elem in lst:
        res &= is_stroked(elem)
    return res


def get_stroked_rows(matrix):
    res = []
    for i in range(0, len(matrix)):
        if is_row_or_column_stroked(matrix[i]):
            res.append(i)
    return res


def get_stroked_columns(matrix):
    res = []
    for j in range(0, len(matrix[0])):
        if is_row_or_column_stroked(get_column(matrix, j)):
            res.append(j)
    return res


def remove_stroke(elem):
    return elem[1:-1]


def update_partly_stroked(matrix):
    min_not_stoked = find_min_in_not_stroked(matrix)
    stroked_rows = get_stroked_rows(matrix)
    stroked_columns = get_stroked_columns(matrix)
    for i in range(0, len(matrix)):
        for j in range(0, len(matrix[0])):
            if not is_stroked(matrix[i][j]):
                matrix[i][j] = str(int(matrix[i][j]) - min_not_stoked)
            elif i in stroked_rows and j in stroked_columns:
                matrix[i][j] = stroke(str(int(remove_stroke(matrix[i][j])) + min_not_stoked))
    return matrix


# Step 9: remove strokes, marks and crosses, and try to get result


def remove_strokes_in_matrix(matrix):
    for i in range(0, len(matrix)):
        for j in range(0, len(matrix[0])):
            if is_stroked(matrix[i][j]):
                matrix[i][j] = remove_stroke(matrix[i][j])
    return matrix


def prettify_null(elem):
    if elem in ["[0]", "x"]:
        return "0"
    return elem


def prettify_matrix(matrix):
    pretty = remove_strokes_in_matrix(matrix)
    for i in range(0, len(pretty)):
        for j in range(0, len(pretty[0])):
            pretty[i][j] = prettify_null(pretty[i][j])
    return pretty


# Result: matrix containing single 1 in each row and columns


def have_result(matrix):
    for i in range(0, len(matrix)):
        zero_index = contains_one_zero(matrix[i], marked=True)
        if zero_index == -1:
            return False
    for j in range(0, len(matrix[0])):
        zero_index = contains_one_zero(get_column(matrix, j), marked=True)
        if zero_index == -1:
            return False
    return True


def create_result(matrix):
    res = []
    for row in matrix:
        new_row = []
        for elem in row:
            if elem == "[0]":
                new_row.append(1)
            else:
                new_row.append(0)
        res.append(new_row)
    return res


def hungarian_algorithm(matrix, debug=False):
    def maybe_print():
        if debug:
            print_matrix(matrix)
    steps_012 = [convert_matrix_to_string, subtract_from_rows, subtract_from_columns]
    for f in steps_012:
        f(matrix)
        maybe_print()
    while True:
        steps_345 = [mark_zeros_in_rows, mark_zeros_in_columns, mark_any_zero]
        for f in steps_345:
            matrix = f(matrix)
            maybe_print()
        if have_result(matrix):
            return create_result(matrix)
        steps_6789 = [stroke_rows, stroke_columns, update_partly_stroked, prettify_matrix]
        for f in steps_6789:
            matrix = f(matrix)
            maybe_print()


test_matrix1 = [[10, 20, 12, 5],
                [3, 14, 9, 1],
                [13, 8, 6, 9],
                [7, 15, 6, 9]]
test_matrix2 = [[6, 15, 3, 12, 4, 2],
                [14, 3, 3, 7, 2, 1],
                [3, 2, 8, 15, 8, 12],
                [3, 14, 3, 15, 11, 10],
                [3, 13, 1, 9, 6, 6],
                [15, 10, 3, 4, 5, 10]]
test_matrix3 = [[6, 5, 3, 9, 12],
                [1, 7, 4, 8, 5],
                [10, 14, 6, 3, 8],
                [11, 5, 9, 4, 8],
                [7, 13, 10, 2, 6]]
test_matrix4 = [[10, 14, 11, 15],
                [3, 20, 19, 17],
                [7, 15, 6, 9],
                [13, 8, 6, 9]]
print_matrix(hungarian_algorithm(test_matrix4, True))
