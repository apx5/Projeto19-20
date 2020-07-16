let read_input = map(read:: String -> Int) . lines

main = do 
    sum . read_input