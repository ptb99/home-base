;;; make-sign.el --- insert username and date (for comments)

;; Author: Tom Pavel <pavel@tr7>
;; Keywords: 


(defun make-signature ()
  (interactive)
  (insert (format "(%s %s)"
		  (user-real-login-name)
		  (current-d-m-y-string))))


;; stolen from headers.el
(defun current-d-m-y-string ()
  (let ((str (current-time-string)))
    (concat (if (equal ?\  (aref str 8))
                       (substring str 9 10)
                       (substring str 8 10))
            "-"
            (substring str 4 7)
            "-"
            (substring str 20 24))))



(provide 'make-sign)
;;; make-sign.el ends here
