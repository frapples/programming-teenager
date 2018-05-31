
(define (make-accumulator start-value)
  (lambda (n)
    (set! start-value (+ start-value n))
    start-value))

(define A (make-accumulator 10))
(A 1)


(define (my-while exp body)
  (define (iter)
    (when (exp)
      (body)
      (iter)))
  (iter))

(define (my-for start-f exp-f end-f body-f)
  (define (iter)
    (when (exp-f)
      (body-f)
      (end-f)
      (iter)))
  (start-f)
  (iter))


(define (test)
  (define i 0)
  (define sum 0)
  (my-while (lambda () (<= i 10))
            (lambda ()
              (set! sum (+ sum i))
              (set! i (+ i 1))))
  sum)

(define (test)
  (define sum 0)
  (define i 0)
  (my-for (lambda () (set! i 0)) (lambda () (<= i 10)) (lambda () (set! i (+ i 1)))
          (lambda () (set! sum (+ sum i))))
  sum)

