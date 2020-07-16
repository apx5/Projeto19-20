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

newtype Benchmark a = Benchmark (String, (a -> ()))

makeBenchmark :: NFData b => String -> (a -> b) -> Benchmark a
makeBenchmark name func = Benchmark (name, \a -> deepseq (func a) ())

benchmarks :: [ Benchmark [Int] ]
benchmarks = [
        makeBenchmark "filter even"               (filter even),
        makeBenchmark "map (*2)"                  (map (*2)),
        makeBenchmark "reverse"                   reverse,
        makeBenchmark "uncurry zip . split id id" (uncurry zip . split id id)
        ]

runBenchmark :: NFData a => a -> Benchmark a -> (String, IO Double)
runBenchmark a (Benchmark (name, func)) = (name, fmap fst . timeItT . (return :: a -> IO a) . deepforce $! a)

benchmarkAll :: NFData a => a -> [ Benchmark a ] -> [(String, IO Double)]
benchmarkAll l = map (runBenchmark l)

printer :: (String, IO Double) -> IO ()
printer (name, iotime) = iotime >>= printer_
        where printer_ time = putStrLn (name ++ ": " ++ printf "%f" (time * 1000) ++ " milliseconds")

main :: IO ()
main = do
        input <- getContents
        let l = readInput input
        let res = benchmarkAll l benchmarks
        forM_ res printer
