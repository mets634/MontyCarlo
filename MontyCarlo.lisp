(defun square (x)
  (* x x))

(defun point-distance (x1 y1 x2 y2)
  (sqrt (+ (square (- x1 x2)) (square (- y1 y2)))))

(defmacro get-x (p)
  `(car ,p))
(defmacro get-y (p)
  `(car (cdr ,p)))

(defun in-circle (p)
  (< (point-distance (get-x p) (get-y p) 0.5 0.5) 0.5))

(defun gen-single-point ()
  `(,(random 1.0) ,(random 1.0)))

(defun gen-n-points(n)
  (loop for k from 1 to n collecting (gen-single-point)))

(defun count-hits (n)
  (length (remove-if-not #'in-circle (gen-n-points n))))

(defun run-expt (n)
  (* 4.0 (/ (count-hits n) n)))