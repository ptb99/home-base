;;;
;;; Tom's streamlined ~/.emacs file
;;;

;; Added by Package.el.  This must come before configurations of
;; installed packages.  Don't delete this line.  If you don't want it,
;; just comment it out by adding a semicolon to the start of the line.
;; You may delete these explanatory comments.
(package-initialize)


;; First, add this to the load path:
(setq load-path (cons (expand-file-name "~/.emacs.d/elisp")
                      load-path))
;; try instead (add-to-list 'load-path "")



;; Custom Settings
;; ===============
(setq custom-file "~/.emacs.d/gnu-emacs-custom")
(load custom-file t t)

;;; Load key bindings regardless of variant:
(load "my-key-bindings" t t)

;; (autoload 'init-c++-buffer "c++-init"
;;   "Add standard headers to a new C++ file." t)
;; (autoload 'twiki-mode "twiki" "Mode for editing Twiki wiki files" t)

;; Use these customizations:
(require 'bracket nil t)           ; "double-quote-word" etc. functions:
(require 'quote-insert nil t)      ; email indent
(require 'make-sign nil t)         ; make-signature function

;; my el to turn on flymake for python:
(require 'python-flymake)


;; why isn't this defined by default?
(defun insert-timestamp ()
  (interactive)
  (insert (current-time-string)))


;; Hack for emacsclient in a separate frame
(add-hook 'server-visit-hook 
          (function (lambda ()
                      ;; first-time through open a new frame, then re-use it
                      (or server-window
                          (setq server-window (make-frame)))
                      (make-frame-visible server-window))))

(add-hook 'server-done-hook 'iconify-frame)

;; start emacsclient server here because it doesn't work in custom settings:
(if (eq window-system 'x)
    (and (server-start)
         (set-face-attribute 'default nil :background "#f5fffa")))



;; Use text-mode for mail
(add-to-list 'auto-mode-alist '("tmpmsg\\." . text-mode))

;; hook in twiki mode
;(add-to-list 'auto-mode-alist'("\\.twiki$" . twiki-mode))


;; Fool around with org-mode
(add-to-list 'auto-mode-alist '("\\.org$" . org-mode))
(define-key global-map "\C-cl" 'org-store-link)
(define-key global-map "\C-ca" 'org-agenda)
;(global-set-key "\C-cb" 'org-iswitchb)
;(org-remember-insinuate)
;(setq org-directory "~/path/to/my/orgfiles/")
;(setq org-default-notes-file (concat org-directory "/notes.org"))
;(define-key global-map "\C-cr" 'org-remember)


;;; C mode
;; (setq c-tab-always-indent t)
;; (setq c-auto-newline nil)
;; (setq c-cleanup-list '(scope-operator brace-else-brace))
;; ;;(setq c-comment-only-line-offset 0)
;; ;;(setq c-hanging-braces-alist nil)

;; (defun my-cc-mode-hook ()
;;   (imenu-add-to-menubar "Imenu"))
;; (add-hook 'c-mode-common-hook 'my-cc-mode-hook)

;; use this to run semantic only when enabling ECB
;; (add-hook 'ecb-before-activate-hook
;;           (lambda () (semantic-mode 1)))

;; This should enable EDE modes:
;(global-ede-mode 1)
;(require 'semantic/sb)
;(require 'semantic/ia)
;(require 'semantic/bovine/gcc)
;(semantic-mode 1)
