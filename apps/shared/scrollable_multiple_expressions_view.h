#ifndef SHARED_SCROLLABLE_MULTIPLE_EXPRESSIONS_VIEW_H
#define SHARED_SCROLLABLE_MULTIPLE_EXPRESSIONS_VIEW_H

#include <escher.h>

namespace Shared {

class AbstractScrollableMultipleExpressionsView : public ScrollableView, public ScrollViewDataSource {
public:
  enum class SubviewPosition : uint8_t {
    Left = 0,
    Center = 1,
    Right = 2
  };
  AbstractScrollableMultipleExpressionsView(Responder * parentResponder, View * contentCell);
  ::EvenOddCell * evenOddCell() {
    return contentCell();
  }
  void setLayouts(Poincare::Layout leftLayout, Poincare::Layout centerlayout, Poincare::Layout rightLayout);
  void setEqualMessage(I18n::Message equalSignMessage);
  SubviewPosition selectedSubviewPosition() {
    return contentCell()->selectedSubviewPosition();
  }
  void setSelectedSubviewPosition(SubviewPosition subviewPosition) {
    contentCell()->setSelectedSubviewPosition(subviewPosition);
  }
  bool displayCenter() const { return constContentCell()->displayCenter(); }
  void setDisplayCenter(bool display);
  void reloadScroll();
  bool handleEvent(Ion::Events::Event event) override;
  Poincare::Layout layout() const {
    return constContentCell()->layout();
  }
protected:
  class ContentCell : public ::EvenOddCell {
  public:
    ContentCell();
    KDColor backgroundColor() const override;
    void setHighlighted(bool highlight) override;
    void setEven(bool even) override;
    void reloadTextColor();
    KDSize minimalSizeForOptimalDisplay() const override;
    virtual ExpressionView * leftExpressionView() const { return nullptr; }
    ExpressionView * rightExpressionView() {
      return &m_rightExpressionView;
    }
    ExpressionView * centeredExpressionView() {
      return &m_centeredExpressionView;
    }
    MessageTextView * approximateSign() {
      return &m_approximateSign;
    }
    SubviewPosition selectedSubviewPosition() const {
      return m_selectedSubviewPosition;
    }
    void setSelectedSubviewPosition(SubviewPosition subviewPosition);
    bool displayCenter() const { return m_displayCenter && !m_centeredExpressionView.layout().isUninitialized(); }
    void setDisplayCenter(bool display);
    void layoutSubviews(bool force = false) override;
    int numberOfSubviews() const override;
    virtual Poincare::Layout layout() const override;

  private:
    View * subviewAtIndex(int index) override;
    ExpressionView m_rightExpressionView;
    MessageTextView m_approximateSign;
    ExpressionView m_centeredExpressionView;
    SubviewPosition m_selectedSubviewPosition;
    bool m_displayCenter;
  };
  virtual ContentCell *  contentCell() = 0;
  virtual const ContentCell *  constContentCell() const = 0;
};

class ScrollableTwoExpressionsView : public AbstractScrollableMultipleExpressionsView {
public:
  ScrollableTwoExpressionsView(Responder * parentResponder) : AbstractScrollableMultipleExpressionsView(parentResponder, &m_contentCell) {
    setMargins(
        Metric::CommonSmallMargin,
        Metric::CommonLargeMargin,
        Metric::CommonSmallMargin,
        Metric::CommonLargeMargin
    );
  }

private:
  ContentCell *  contentCell() override { return &m_contentCell; };
  const ContentCell *  constContentCell() const override { return &m_contentCell; };
  ContentCell m_contentCell;
};


}

#endif
