(defun bracket-word (arg string1 string2)
  "Put STRING1 at the beginning of the current word and put STRING2 the end
 of the ARGth from current word"
  (save-excursion
    (or (looking-at "\\<") (forward-word -1))
    (insert string1)
    (forward-word arg)
    (insert string2)))

(defun double-quote-word (arg)
  "Put a `\"' at the beginning of the current word and at the end of the
 ARGth word from point"
  (interactive "p")
  (bracket-word arg "\"" "\""))

(defun single-quote-word (arg)
  "Put a \"'\" at the beginning of the current word and at the end of the
 ARGth word from point"
  (interactive "p")
  (bracket-word arg "'" "'"))

(defun paren-word (arg)
  "Put parentheses at the beginning of the current word and at the end of the
 ARGth word from point"
  (interactive "p")
  (bracket-word arg "(" ")"))

(defun curly-paren-word (arg)
  "Put braces at the beginning of the current word and at the end of the
 ARGth word from point"
  (interactive "p")
  (bracket-word arg "{" "}"))

(defun square-paren-word (arg)
  "Put brackets at the beginning of the current word and at the end of the
 ARGth word from point"
  (interactive "p")
  (bracket-word arg "[" "]"))

(defun dollar-sign-word (arg)
  "Put dollar-signs at the beginning of the current word and at the
  end of the ARGth word from point (for TeX)"
  (interactive "p")
  (bracket-word arg "$" "$"))

;; Now for some key definitions
(define-key global-map "\e\"" 'double-quote-word)
(define-key global-map "\e'" 'single-quote-word)

(provide 'bracket)
