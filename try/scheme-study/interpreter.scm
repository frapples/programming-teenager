(use-modules (ice-9 match))

(let ([x 1]
      [y 1])
  (+ x y))

(define (tree-sum exp)
  (cond [(number? exp) exp]
        [(null? exp) 0]
        [else (+
               (tree-sum (car exp))
               (tree-sum (cdr exp)))]))

(define tree-sum
  (lambda (exp)
    (match exp                         ; 对输入exp进行模式匹配
      [(? number? x) x]                ; exp是一个数x吗？如果是，那么返回这个数x
      [`(,e1 ,e2)                      ; exp是一个含有两棵子树的中间节点吗？
       (let ([v1 (tree-sum e1)]        ; 递归调用tree-sum自己，对左子树e1求值
             [v2 (tree-sum e2)])       ; 递归调用tree-sum自己，对右子树e2求值
         (+ v1 v2))])))

(define (calc exp)
  (match exp
    [(? number? x) x]
    [`(,op ,e1 ,e2)
     (let ([v1 (calc e1)]
           [v2 (calc e2)])
       (match op
         ['+ (+ v1 v2)]
         ['- (+ v1 v2)]
         ['* (+ v1 v2)]
         ['/ (+ v1 v2)]
         ))]))

(calc '(+ 1 3))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define env0 '())
(define (ext-env x v env)
  (cons `(,x . ,v) env))

(define (lookup x env)
  (let ([p (assq x env)])
    (cond
     [(not p) #f]
     [else (cdr p)])))

;; 闭包数据对象
(define (make-closure exp env)
  (cons 'closure (cons exp env)))
(define (closure? v)
  (eq? (car v) 'closure))

(define (closure-exp closure)
  (cadr closure))

(define (closure-env closure)
  (cddr closure))

(define (interp exp env)
  (match exp
    [(? symbol? x)
     (let ([v (lookup x env)])
       (if (not v)
           (error "未定义的变量" x)
           v))]
    [(? number? x) x]
    [`(lambda (,x) ,e)
     (make-closure exp env)]
    [`(let ([,x ,e1]) ,e2)                            ; 绑定
     (let ([v1 (interp e1 env)])
       (interp e2 (ext-env x v1 env)))]
    [`(,e1 ,e2)                                       ; 调用
     (let ([v1 (interp e1 env)]
           [v2 (interp e2 env)])
       (when (closure? v1)
         (let ([f-exp (closure-exp v1)]
               [env-save (closure-env v1)])
           (match f-exp
             [`(lambda (,x) ,e)
              (interp e (ext-env x v2 env-save))]
             ))))]
    [`(,op ,e1 ,e2)                                   ; 算术表达式
     (let ([v1 (interp e1 env)]
           [v2 (interp e2 env)])
       (match op
         ['+ (+ v1 v2)]
         ['- (- v1 v2)]
         ['* (* v1 v2)]
         ['/ (/ v1 v2)]))]
    ))


(define (r2 exp)
  (interp exp env0))


(r2
 '(let ([x 2])
    (let ([f (lambda (y) (* x y))])
      (let ([x 4])
        (f 3)))))
