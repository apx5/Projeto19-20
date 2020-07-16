(import chicken.process-context chicken.random srfi-1)

(let ((max (string->number (car (command-line-arguments)))))
  (let loop ((i 0))
    (when (< i max)
      (print (pseudo-random-integer 1000000))
      (loop (+ 1 i)))))
