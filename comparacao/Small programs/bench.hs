module Main where

import Data.List
import Control.Monad
import Control.DeepSeq
import System.TimeIt
import Text.Printf

readInput :: String -> [Int]
readInput input = seq readLines (map (read :: String -> Int) readLines)
    where readLines = lines input

doall :: String -> Int
doall input = seq readLines (foldl (\r x -> r + (read x)) 0 readLines)
    where readLines = lines input

benches :: [ (String, [Int] -> [Int]) ]
benches = [
        ("map (*2)", map (*2)),
        ("filter even", filter even)
        ]

force x = seq x x
deepforce x = deepseq x x

benchAll :: [ (String, [Int] -> [Int]) ] -> [Int] -> [(String, IO (Double, [Int]))]
benchAll benches l = do
        (name, bench) <- benches
        let x = bench l
        let t = timeItT ((return :: a -> IO a) $! deepforce x)
        return (name, t)

main = do
        input <- getContents
        let l = readInput input
        let res = l `deepseq` benchAll benches l
        forM_ res (\(name, r) -> do
                (time, _) <- r
                print (name ++ ": " ++ printf "%f" (time * 1000) ++ " milliseconds"))
