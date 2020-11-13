#ifndef SLIPWT_H
#define SLIPWT_H

#include <QWidget>
class QPushButton;
class QPropertyAnimation;
//class QLabel;


class Slipwt : public QWidget
{
    Q_OBJECT
public:
    explicit Slipwt(QWidget *parent = nullptr);
    void SetChecked(const bool ischecked);
    void SetColors(const QString& background_normal = "#ababab", const QString& background_checked = "#00ff00", const QString& btn = "#ffffff");

protected:
    void resizeEvent(QResizeEvent * e) override;
    void paintEvent(QPaintEvent *) override;

private:
    QPushButton *slipbtn_;
    QPropertyAnimation* ani_;

    QString background_normal_color_;
    QString background_checked_color_;
    QString btn_color_;

    bool ischecked_ = false;
    void UpdateStyleSheet();

signals:
    void SignalCheckedChanged(const bool ischecked);

private slots:
    void SlotBtnClicked();

};

#endif // SLIPWT_H
