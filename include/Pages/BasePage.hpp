#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

class BasePage : public QWidget {
    Q_OBJECT

public:
    explicit BasePage(QStackedWidget* stackedWidget, QWidget* parent = nullptr)
        : QWidget(parent)
        , m_stackedWidget(stackedWidget)
        , m_layout(new QVBoxLayout(this))
        , m_titleLabel(new QLabel(this))
    {
        // Setup title label
        QFont titleFont = m_titleLabel->font();
        titleFont.setPointSize(16);
        titleFont.setBold(true);
        m_titleLabel->setFont(titleFont);
        m_titleLabel->setAlignment(Qt::AlignCenter);
        m_layout->addWidget(m_titleLabel);
    }

protected:
    QPushButton* createMenuButton(const QString& text) {
        QPushButton* button = new QPushButton(text, this);
        button->setMinimumHeight(40);
        button->setFont(QFont("Arial", 12));
        return button;
    }

    QStackedWidget* m_stackedWidget;
    QVBoxLayout* m_layout;
    QLabel* m_titleLabel;
}; 