#ifndef PAGINGTOOLBAR_H
#define PAGINGTOOLBAR_H

/*
* @brief:  分页栏，
*           允许左对齐、右对齐、居中对齐；
*           允许自动隐藏翻页按钮
* @author: LastSee
* @date: 2020
*
*/

#include <QWidget>
class QPushButton;
//class QHBoxLayout;
class QLabel;



class PagingToolBar : public QWidget
{
    Q_OBJECT

public:
    PagingToolBar(QWidget *parent = nullptr);
    void Init(const int index_count, const bool auto_hide_btn_prev_next, const Qt::AlignmentFlag align = Qt::AlignRight);
    void SetCurPage(const int cur_page);    //样式：  “上一页” 1 2 3 ... “下一页”
    void SetMaxPage(const int maxpage);
    int GetMaxPage() const;
    int GetCurPage() const;
    ~PagingToolBar();

private:
    QList<QPushButton*> btns_{};
    QLabel *max_page_lb_{};

    int max_btn_index_ = 0;
    int maxpage_ = 0;
    int cur_page_ = 1;
    int cur_btn_index_ = 0;
    QWidget *btn_wt_ = nullptr;
    bool auto_hide_btn_prev_next_ = false;
    int btn_prev_next_strech_ = 13;
    Qt::AlignmentFlag btn_index_aligin_ = Qt::AlignRight; // Qt::AlignLeft; AlignCenter Qt::AlignRight; 当按钮较少时候，整体右对齐


signals:
    void SignalCurPageChanged(const int curpage);

public slots:

private slots:
    void SlotBtnClicked();
};
#endif // PAGINGTOOLBAR_H
