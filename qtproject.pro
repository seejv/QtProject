#CONFIG += ordered

SUBDIRS = \
        QtVirtualKeyboard/useKeyBoard_Demo \
        QtVirtualKeyboard/makeChinesePY_db \
        QtPagingToolBar/PagingToolBar \
        QtSliptBotton/SliptBotton \
        QtDrawingBoard/DrawingBoard \
        QtTimer/Timer

## 设置构建目录为 build 目录
#for(SUBDIR, SUBDIRS) {
#  $${SUBDIR}.target = $${SUBDIR}
#  $${SUBDIR}.target.path = $$OUT_PWD/build/$${SUBDIR}
#}

TEMPLATE = subdirs
