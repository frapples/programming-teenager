(define f
  (lambda (self n)
    (if (= n 0)
        1
        (* (self self (- n 1)) n))))

(f f 3)

(define Y
  (lambda (f)
    (lambda (. args) (apply f (cons f args)))))


((Y f) 3)

((Y (lambda (self n)
    (if (= n 0)
        1
        (* (self self (- n 1)) n)))) 5)
