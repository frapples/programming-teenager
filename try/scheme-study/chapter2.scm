;;;;;;;;;;;;;;;;;;;; 有理数的运算

(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (remainder a b))))


;;;; 这是一组构造函数和选择函数，它们会直接关心表示数据的细节

;; 我们要把生成的有理数化为最简形式。有两种方案：在构造函数里化简，或选择函数里。

(define (make-rat n d)
  (let ((g (gcd n d)))
    (cons (/  n g) (/ d g))))

(define (numer x)
  (car x))

(define (denom x)
  (cdr x))


;; 这是一组操作数据的函数，通过也只通过构造函数和选择函数来达到目的
(define (add-rat x y)
  (make-rat (+ (* (numer x) (denom y))
               (* (numer y) (denom x)))
            (* (denom x) (denom y))))

(define (mul-rat x y)
  (make-rat (* (numer x) (numer y))
            (* (denom x) (denom y))))

(define (equal-rat? x y)
  (= (* (numer x) (denom y))
     (* (numer y) (denom x))))

(add-rat (make-rat 1 6) (make-rat 1 3))

(mul-rat (make-rat 2 5) (make-rat 1 2))



;;;;;;;;;;;;;;;;;;;;;;;;;;;; 符号求导
(define (variable? v)
  (symbol? v))
(define (expression? v)
  (or (list? v) (variable? v)))
(define (same-variable? v1 v2)
  (equal? v1 v2))

;;;

(define (sum? e)
  (eq?  (car e) '+))
(define (addends e)
  (cdr e))

(define (product? e)
  (eq? (car e) '*))
(define (multiplicands e)
  (cdr e))

(define (make-sum e1 e2)
  (make-sum-list (list e1 e2)))

(define (make-sum-list exp-list)
  (let ((nozero-exp-list (filter (lambda (x) (not (eq? x 0))) exp-list)))
    (cond ((= (length nozero-exp-list) 0) 0)
          ((= (length nozero-exp-list) 1) (car nozero-exp-list))
          (else (cons '+ nozero-exp-list)))))

(define (make-product e1 e2)
  (define eq)
  (cond ((or (eq? e1 0) (eq? e2 0)) 0)
        ((eq? e1 1) e2)
        ((eq? e2 1) e1)
        (else (list '* e1 e2))))
;;;


(define (deriv exp var)
  (cond [(number? exp) 0]
        [(same-variable? exp var) 1]
        [(and (variable? exp) (variable? var) (not (same-variable? exp var)))
         0]
        [(sum? exp)
         (make-sum-list
          (map
           (lambda (exp)
             (deriv exp var))
           (addends exp)))]
        [(product? exp)
         (make-sum
          (make-product (car (multiplicands exp)) (deriv (cadr (multiplicands exp)) var))
          (make-product (cadr (multiplicands exp)) (deriv (car (multiplicands exp)) var))
          )]
        ))

(deriv '(+ (* x x) x 1) 'x)

;;;;;;;;;;;;;;;;;;;; 复数

