#pragma once

#ifndef FILLTOOL_H
#define FILLTOOL_H

// TnzCore includes
#include "tproperty.h"
#include "toonz/txshlevelhandle.h"
#include "toonz/txshsimplelevel.h"
// TnzTools includes
#include "tools/tool.h"
#include "tools/toolutils.h"
#include "autofill.h"
#include "toonz/fill.h"
#include "vectorbrush.h"
#include "symmetrystroke.h"

#include <QObject>

#define LINES L"Lines"
#define AREAS L"Areas"
#define ALL L"Lines & Areas"

#define IGNOREGAPS L"Ignore Gaps"
#define FILLGAPS L"Fill Gaps"
#define CLOSEANDFILLGAPS L"Close and Fill"

class NormalLineFillTool;
namespace {

class AreaFillTool {
public:
  enum Type { RECT, FREEHAND, POLYLINE, FREEPICK };

private:
  bool m_frameRange;
  bool m_onlyUnfilled;
  Type m_type;

  bool m_selecting;
  TRectD m_selectingRect;

  TRectD m_firstRect;
  bool m_firstFrameSelected;
  TXshSimpleLevelP m_level;
  TFrameId m_firstFrameId, m_veryFirstFrameId;
  TTool *m_parent;
  std::wstring m_colorType;
  std::pair<int, int> m_currCell;
  VectorBrush m_track;
  SymmetryStroke m_polyline;
  bool m_isPath;
  bool m_active;
  bool m_enabled;
  double m_thick;
  TPointD m_firstPos;
  std::vector<TStroke *> m_firstStrokes;
  TPointD m_mousePosition;
  bool m_onion;
  bool m_isLeftButtonPressed;
  bool m_autopaintLines;
  bool m_fillOnlySavebox;

  int m_bckStyleId;

public:
  AreaFillTool(TTool *Parent);
  void draw();
  int pick(const TImageP &image, const TPointD &pos, const int frame, int mode);
  void resetMulti();
  void leftButtonDown(const TPointD &pos, const TMouseEvent &, TImage *img);
  void leftButtonDoubleClick(const TPointD &pos, const TMouseEvent &e,
                             bool fillGaps, bool closeGaps,
                             int closeStyleIndex);
  void leftButtonDrag(const TPointD &pos, const TMouseEvent &e);
  void mouseMove(const TPointD &pos, const TMouseEvent &e);
  void leftButtonUp(const TPointD &pos, const TMouseEvent &e, bool fillGaps,
                    bool closeGaps, int closeStyleIndex);
  void onImageChanged();
  bool onPropertyChanged(bool multi, bool onlyUnfilled, bool onion, Type type,
                         std::wstring colorType, bool autopaintLines,
                         bool fillOnlySavebox);
  void onActivate();
  void onEnter();
};
}  // namespace
class FillTool final : public QObject, public TTool {
  // Q_DECLARE_TR_FUNCTIONS(FillTool)
  Q_OBJECT
  bool m_firstTime;
  TPointD m_firstPoint, m_clickPoint;
  bool m_firstClick;
  bool m_frameSwitched             = false;
  double m_changedGapOriginalValue = -1.0;
  TXshSimpleLevelP m_level;
  TFrameId m_firstFrameId, m_veryFirstFrameId;
  int m_onionStyleId;
  TEnumProperty m_colorType;  // Line, Area
  TEnumProperty m_fillType;   // Rect, Polyline etc.
  TBoolProperty m_onion;
  TBoolProperty m_frameRange;
  TBoolProperty m_selective;
  TDoublePairProperty m_fillDepth;
  TBoolProperty m_segment;
  TDoubleProperty m_maxGapDistance;
  TDoubleProperty m_rasterGapDistance;
  TEnumProperty m_closeRasterGaps;
  TStyleIndexProperty m_closeStyleIndex;
  AreaFillTool *m_rectFill;
  NormalLineFillTool *m_normalLineFillTool;
  TBoolProperty m_referenced;

  TPropertyGroup m_prop;
  std::pair<int, int> m_currCell;
  std::vector<TFilledRegionInf> m_oldFillInformation;
#ifdef _DEBUG
  std::vector<TRect> m_rects;
#endif

  // For the raster fill tool, autopaint lines is optional and can be temporary
  // disabled
  TBoolProperty m_autopaintLines;
  TBoolProperty m_fillOnlySavebox;

  int m_firstFrameIdx, m_lastFrameIdx;

public:
  FillTool(int targetType);

  ToolType getToolType() const override { return TTool::LevelWriteTool; }

  void updateTranslation() override;

  TPropertyGroup *getProperties(int targetType) override { return &m_prop; }

  FillParameters getFillParameters() const;

  void leftButtonDown(const TPointD &pos, const TMouseEvent &e) override;
  void leftButtonDrag(const TPointD &pos, const TMouseEvent &e) override;
  void leftButtonUp(const TPointD &pos, const TMouseEvent &e) override;
  void mouseMove(const TPointD &pos, const TMouseEvent &e) override;
  void leftButtonDoubleClick(const TPointD &pos, const TMouseEvent &e) override;
  void resetMulti();

  bool onPropertyChanged(std::string propertyName) override;
  void onImageChanged() override;
  void draw() override;

  // if frame = -1 it uses current frame
  int pick(const TImageP &image, const TPointD &pos, const int frame = -1);
  int pickOnionColor(const TPointD &pos);

  void onEnter() override;

  void onActivate() override;
  void onDeactivate() override;

  int getCursorId() const override;

  int getColorClass() const { return 2; }

private:
  void applyFill(const TImageP &img, const TPointD &pos, FillParameters &params,
                 bool isShiftFill, TXshSimpleLevel *sl, const TFrameId &fid,
                 bool autopaintLines, bool fillGaps = false,
                 bool closeGaps = false, int closeStyleIndex = -1,
                 int frameIndex = -1);

public slots:
  void onFrameSwitched() override;
};

#endif  // FILLTOOL_H
