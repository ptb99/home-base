# .cshrc initialization
#  -  this is a tcsh script (sometimes)
#

### env-vars and things that inherit
if (! $?ENVSET && -f ${HOME}/.envcsh) source ${HOME}/.envcsh

### If process is not interactive, don't bother with the rest:
if ($?prompt == 0) exit         


# Decide if we are really tcsh or not:
if ($?tcsh) then
    set prompt="%m[%\!]% "
        
    #set keys (now defaults?)
    #bindkey "^w" backward-delete-word
    #bindkey "^u" backward-kill-line
else
    setenv SHELL /bin/csh   # in case tcsh is a link to csh
    set prompt="`hostname`[\!]% "
endif


# shell variable that control stuff:
set history = 1000
set savehist = (1000 merge)
set noclobber
set notify
set filec
set ignoreeof
#set fignore = ()
#set mail = (/var/mail/$USER)
set time=(10 "%Uu %Ss %E %Mrss+%Xsh+%Ddat+%Kstack %I+%Oio %Fpf+%Ww")
        #show time for jobs taking longer than 10 CPU sec.

#Tcsh special vars (shouldn't do any harm to csh):
set autoexpand
set dextract
set dunique
set showdots
set listflags = a
set listlinks
set rmstar
set echo_style = both
set listjobs = long
set nostat = (/afs /nfs)

## Linux default compiles have this set
unset autologout

# convenience variable for foreach loops:
set ten = (0 1 2 3 4 5 6 7 8 9)


# Start of aliases:
alias dir "ls -Fsk"
#alias lsl "ls -alg"
alias lsl "ls -al"
alias em  "emacs -nw"
alias h history
alias j "jobs -l"
alias m less
alias nman "tbl \!* | neqn -Tascii | nroff -man | col | less"
alias purge "rm -rf *~ .*~ #*# *.CKP"
alias pd pushd
alias pop popd
alias ppr 'enscript -2r --margins=72:::36 -F Times-RomanBold12 -P${PRINTER_Postscript}'
alias ssh-fix "ssh-keygen -f ~/.ssh/known_hosts -R "
alias ac apt-cache
alias ag apt-get

alias slac "ssh -l pavel flora04.slac.stanford.edu"
#alias work "ssh -L 8888:cobalt-sup:80 -L 5900:shelby:5901 pavel@shost.networkphysics.com"
#alias pwd 'echo $cwd'
#alias hd "hexdump -C"

#alias tset     'set noglob histchars=""; eval `\tset -s \!*`; unset noglob histchars'
#alias psr 'lpr -P${PRINTER_Postscript} '
#alias psrm 'lprm -P${PRINTER_Postscript} '
#alias psq 'lpq -P${PRINTER_Postscript} '


### Completions customization
# skip for now
#if ($?tcsh && -f ${HOME}/.complete) source ${HOME}/.complete


