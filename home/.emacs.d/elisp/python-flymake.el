;;; python-flymake.el --- hooks for flymake on python sources

;; Author: Tom Pavel <tpavel@ls7>
;; Keywords: 


(require 'flymake)

(defun flymake-pyflakes-init ()
  ;; Make sure it's not a remote buffer or flymake would not work
  (unless (and (functionp 'tramp-list-remote-buffers)
               (memq (current-buffer) (tramp-list-remote-buffers)))
    (let* ((temp-file (flymake-init-create-temp-buffer-copy
                       'flymake-create-temp-inplace))
           (local-file (file-relative-name
                        temp-file
                        (file-name-directory buffer-file-name))))
      (list "pyflakes" (list local-file)))))

(add-to-list 'flymake-allowed-file-name-masks
             '("\\.py\\'" flymake-pyflakes-init))

(add-hook 'python-mode-hook 'flymake-find-file-hook)


(provide 'python-flymake)
;;; python-flymake.el ends here
