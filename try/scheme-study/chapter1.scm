(define (add-max1-max2 a b c)
  (cond ((and (< a b) (< a c)) (+ b c))
        ((and (< b a) (< b c)) (+ a c))
        ((and (< c a) (< c b)) (+ a b))))

(define (f-1 n)
  (if (< n 3)
      n
      (+
       (f-1 (- n 1))
       (* 2 (f-1 (- n 2)))
       (* 3 (f-1 (- n 3)))
       )))

(define (f-2 n)
  (define (f-2-iter n-1 n-2 n-3 count)
    (if (= count 2)
        n-1
        (f-2-iter (+ n-1 (* 2 n-2) (* 3 n-3)) n-1 n-2 (- count 1))))
  (if (< n 3)
      n
      (f-2-iter 2 1 0 n)))

(define (square x)
  (* x x))

(define (even? n)
  (= (remainder n 2) 0))

(define (fast-expt b n)
  (cond ((= n 0) 1)
        ((even? n) (square (fast-expt b (/ n 2))))
        (else (* b (fast-expt b (- n 1))))))

; 1.16
(define (fast-expt b n)
  (define (fast-expt-iter a b n)
    (cond ((= n 0) a)
          ((even? n) (fast-expt-iter a (square b) (/ n 2)))
          (else (fast-expt-iter (* a b) b (- n 1)))))
  (fast-expt-iter 1 b n))

(fast-expt 2 10)


(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (remainder a b))))


; 高阶函数
(define (sum term a next b)
  (if (> a b)
      0
      (+ (term a) (sum term (next a) next b))))


(define (sum-intergers a b)
  (sum (lambda (n) n) a (lambda (n) (+ n 1)) b))

(sum-intergers 1 2)

; 1.30
(define (sum term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (+ result (term a)))))
  (iter a 0))

; 1.29
(define (integral f a b n)
  (let ((h (/ (- b a) n)))
    (* (/ h 3)
       (sum
        (lambda (k)
          (*
           (cond ((or (= k 0) (= k n)) 1)
                 ((even? k) 2)
                 (else 4))
           (f (+ a (* k h)))))
        0
        (lambda (k) (+ k 1))
        n
        ))))

(integral (lambda (x) (* x x x)) 0 1 100)


(define (close-enough? a b)
  (< (abs (- a b)) 0.00001))

(define (fixed-point f first-guess)
  (define (iter guess-1 guess-2)
    (if (close-enough? guess-1 guess-2)
        guess-2
        (iter guess-2 (f guess-2))))
  (iter first-guess (f first-guess)))



(fixed-point (lambda (x) (+ (sin x) (cos x))) 1.0)

; a/x = x
(define (my-sqrt x_)
  (fixed-point (lambda (x) (/ (+ (/ x_ x) x) 2)) 1.0))

(my-sqrt 2.0)

; 1.37
(define (cont-frac n-f d-f k)
  (if (= k 0)
      0
      (/ (n-f 1) (+ (d-f 1)
                    (cont-frac (lambda (i) (n-f (+ i 1)))
                               (lambda (i) (d-f (+ i 1)))
                               (- k 1))))))

(cont-frac (lambda (i) 1.0) (lambda (i) 1.0) 100)

; 1.38
(cont-frac
 (lambda (i) 1.0)
 (lambda (i) (let ((i-mod-3 (remainder i 3)))
               (cond ((= i-mod-3 0) 1)
                     ((= i-mod-3 1) 1)
                     ((= i-mod-3 2) (* 2 (+ (floor (/ i 3)) 1))))))
 100)


; 1.39
(define (tan-cf x k)
  (cont-frac
   (lambda (i) (if (= i 1) x (* x x)))
   (lambda (i) (- (* 2 i) 1))
   k))

(tan-cf (/ 3.1415926 3) 100)

; 章节1.3.4

; 随便写的
(define (merge lst1 lst2)
  (cond ((eq? lst1 '()) lst2)
        ((eq? lst2 '()) lst1)
        ((<= (car lst1) (car lst2))
         (cons (car lst1) (merge (cdr lst1) lst2)))
        ((> (car lst1) (car lst2))
         (cons (car lst2) (merge (cdr lst2) lst1)))))


(define (merge lst1 lst2)
  (define (iter sublst1 sublst2 result)
    (cond ((and (eq? sublst1 '()) (eq? sublst2 '()))
           result)
          ((eq? sublst1 '())
           (append result sublst2))
          ((eq? sublst2 '())
           (append result sublst1))
          ((<= (car sublst1) (car sublst2))
           (iter (cdr sublst1) sublst2 (append result (list (car sublst1)))))
          ((> (car sublst1) (car sublst2))
           (iter sublst1 (cdr sublst2) (append result (list (car sublst2)))))))
  (iter lst1 lst2 '()))

(merge '(1 3 5 6) '(2 5 7 9))

(define (test-case x)
  (case x
    ((1 3 5 7 8 10 12) 31)
    ((2) 28)
    ((4 6 9 11) '30)))

(test-case 6)
