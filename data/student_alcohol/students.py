

# read first line (column data)
column_names = input()
print(column_names)

while True:

  try:
    line = input().split(" ")
    last_col = int(line[-1])
    is_alch = "no" if last_col <= 2 else "yes"
    out = ' '.join(line[:len(line)-1]) + " " + is_alch
    print(out)
  except Exception as e:
    break


