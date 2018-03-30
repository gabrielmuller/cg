if &cp | set nocp | endif
nnoremap 	 ==j
nnoremap 0 ^
nnoremap N Nzzzv
nnoremap Y y$
let s:cpo_save=&cpo
set cpo&vim
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
onoremap <silent> <expr> j v:count ? 'j' : 'gj'
onoremap <silent> <expr> k v:count ? 'k' : 'gk'
nnoremap n nzzzv
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
onoremap <silent> <expr> <Down> v:count ? '<Down>' : 'g<Down>'
onoremap <silent> <expr> <Up> v:count ? '<Up>' : 'g<Up>'
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=indent,eol,start
set clipboard=unnamedplus,autoselect,exclude:cons\\|linux
set confirm
set directory=~/.vim/swap/
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set helplang=pt
set mouse=a
set ruler
set shiftwidth=4
set shortmess=filnxtToOaAcIws
set softtabstop=4
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc,.png,.jpg
set termguicolors
set wildmenu
" vim: set ft=vim :
