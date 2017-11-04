;; Key Definitions:

;(define-key global-map "\C-x\C-b" 'buffer-menu)
(define-key global-map "\eq" 'query-replace)
(define-key global-map "\er" 'query-replace)
(define-key global-map "\es" 're-search-forward)
(define-key global-map "\ep" 'fill-paragraph)
(define-key global-map "\e%" 'goto-line)
(define-key global-map "\C-xm" 'mh-rmail)
(define-key global-map "\C-xc" 'compile)
(define-key global-map "\C-x\C-r" 'revert-buffer)
(define-key global-map "\C-c\C-c" 'server-edit)
(define-key global-map "\C-cr" 'comment-region)
(define-key global-map "\C-cq" 'quote-insert)
(define-key global-map "\C-ci" 'indent-region)
;(global-unset-key "\e\e")    ;Avoid the "disabled" message for typos

;; stick with the old keybinding for calc (is this needed??):
(global-set-key "\e#" 'calc-dispatch)

;; Having this be apropos just confuses me...
(define-key help-map "d" 'describe-function)

;;; Turn off disabling of useful commands
;(put 'downcase-region 'disabled nil)
;(put 'upcase-region 'disabled nil)
;(put 'eval-expression 'disabled nil)


