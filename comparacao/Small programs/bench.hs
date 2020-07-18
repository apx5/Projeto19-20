{-# LANGUAGE BangPatterns #-}

module Main where

import Control.DeepSeq
import Control.Monad.IO.Class (MonadIO(liftIO))
import System.CPUTime
import Text.Printf

split f g x = (f x, g x)
deepforce x = deepseq x x

readInput :: String -> [Int]
readInput input = map (read :: String -> Int) readLines
    where readLines = lines input

-- Adapted from https://github.com/stackbuilders/reverse/blob/fa120ba688aea328a587cbb1971280c22085a257/benchmarks/ClockBenchmarks.hs
timeSomething :: NFData a => String -> a -> IO ()
timeSomething str something = do
  start <- liftIO getCPUTime
  let !result = deepforce $! something
  end <- liftIO getCPUTime
  let diff = round $ (fromIntegral $ end - start) * 1e-3
  putStrLn $ str ++ ": " ++ show diff ++ " nanoseconds"

main :: IO ()
main = do
    input <- getContents
    let !l = deepforce $ readInput input

    timeSomething "filter even"               $ filter even l
    timeSomething "map (*2)"                  $ map (*2) l
    timeSomething "reverse"                   $ reverse l
    timeSomething "uncurry zip . split id id" $ zip l l
