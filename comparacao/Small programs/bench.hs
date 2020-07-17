module Main where

import Control.DeepSeq
import Control.Monad
import Data.List
import System.TimeIt
import Text.Printf

split f g x = (f x, g x)
force x = seq x x
deepforce x = deepseq x x

readInput :: String -> [Int]
readInput input = map (read :: String -> Int) readLines
    where readLines = lines input

newtype Benchmark = Benchmark (String, IO ())

makeBenchmark :: NFData b => String -> a -> (a -> b) -> Benchmark
makeBenchmark name a func = Benchmark (name, return . const () . deepforce . func $ a)

benchmarks :: [Int] -> [ Benchmark ]
benchmarks l = [
        makeBenchmark "filter even"               l (filter even),
        makeBenchmark "map (*2)"                  l (map (*2)),
        makeBenchmark "reverse"                   l reverse,
        makeBenchmark "uncurry zip . split id id" l (uncurry zip . split id id)
        ]

runBenchmark :: Benchmark -> (String, IO Double)
runBenchmark (Benchmark (name, func)) = (name, fmap fst $ timeItT func)

benchmarkAll :: [ Benchmark ] -> [(String, IO Double)]
benchmarkAll = map runBenchmark

printer :: (String, IO Double) -> IO ()
printer (name, iotime) = iotime >>= printer_
        where printer_ time = putStrLn (name ++ ": " ++ printf "%f" (time * 1000) ++ " milliseconds")

main :: IO ()
main = do
        input <- getContents
        let l = readInput input
        let results = benchmarkAll . benchmarks $! l
        forM_ results printer
