;; No startupscreen
(setq inhibit-startup-screen t)

;; Line numbers
(global-linum-mode 1)

;; Delete selection
(delete-selection-mode t)

;; No newlines automatically
(setq next-line-add-newlines nil)

;; Use '*.h' as C++, not C
(add-to-list 'auto-mode-alist '("\\.h$" . c++-mode))

;; Bra indrag
(setq c-default-style "ellemtel")
(setq-default indent-tabs-mode nil)

;; Indenteringsdjup
(setq c-basic-offset 2)

;; Markera allt över 80 tecken långt
(custom-set-faces
   '(my-long-line-face ((((class color)) (:background "grey70"))) t))

(add-hook 'font-lock-mode-hook
            (function
             (lambda ()
               (setq font-lock-keywords
                     (append font-lock-keywords
                             '(("\t+" (0 'my-tab-face t))
                               ("^.\\{81,\\}$" (0 'my-long-line-face t))
                               ("[ \t]+$"      (0 'my-trailing-space-face t))))))))


(nconc load-path '("ruby-1.9-emacs"))
(require 'ruby-site)
