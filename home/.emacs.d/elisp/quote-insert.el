;;; quote-insert.el --- insert quotes for email replies

;; Author: Tom Pavel <pavel@tr7>
;; Keywords: 


(defun quote-insert (beg end)
  "Simple function for email.  Insert a \"> \" citation string at
  every line in the current region."
  (interactive "r")
  (save-excursion
    (goto-char end)
    (and (bolp) (goto-char (1- end)))
    (while (<= beg (point))
      (move-beginning-of-line 1)
      (insert "> ")
      (forward-line -1))))


(provide 'quote-insert)
;;; quote-insert.el ends here
