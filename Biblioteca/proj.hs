add:: Num a => a -> a -> a
add a b = a + b

addL:: [Int] -> Int
addL [] = 0
addL (h:t) = h + addL t

max' :: (Num a, Ord a) => a -> a -> a
max' a b | a > b = a
        | otherwise = b
        

filter' :: (a -> Bool) -> [a] -> [a]
filter' f [] = []
filter' f (h:t)  | f h = h: filter' f t
                | otherwise = filter' f t
                
map' :: (a -> b) -> [a] -> [b]
map' f [] = []
map' f (h:t) = f h : map' f t

foldl :: Foldable t => (b -> a -> b) -> b -> t a -> b

foldl f b [] = b
foldl f b (h:t) = foldl f (f b h) t         

zip:: [a] -> [b] -> [(a,b)]
zip = zipWith (,)

zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
zipWith f [] _ = []
zipWith f _ [] = []
zipWith f (h:t) (x:xs) = (f h x) : zipWith f t xs

iterate:: (a -> a) -> a -> [a]
iterate f a = a: iterate f (f a)