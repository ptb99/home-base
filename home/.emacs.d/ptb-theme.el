(deftheme ptb
  "My custom theme - Created 2014-10-28.")

;(set-face-attribute 'default nil :background "#f5fffa")

(custom-theme-set-faces
 'ptb
 '(default ((t (:background "#f5fffa"))))
 '(highlight ((t (:background "yellow"))))
 '(region ((t (:background "yellow")))))


;; '(default ((t (:inherit nil :stipple nil :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 98 :width normal :foundry "bitstream" :family "Bitstream Vera Sans Mono")))))



(provide-theme 'ptb)
