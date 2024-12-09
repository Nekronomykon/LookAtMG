#ifndef Tag_File_Format_h__
#define Tag_File_Format_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <string>
using String = std::string;

#include <utility>
#include <algorithm>
#include <filesystem>

#include <QString>
#include <QMessageBox>

namespace fs = std::filesystem;
using Path = fs::path;
// using Path = String;

template <class Host>
class TagFileFormat
{
public:
  typedef bool (Host::*DataParser)(Path);
  typedef bool (*FileParser)(Path, Host &);
  typedef bool (Host::*DataWriter)(Path);
  typedef bool (*FileWriter)(Host &, Path);

  explicit TagFileFormat(String nameFmt = String(),
                         String ext = String(),
                         DataParser read = nullptr,
                         DataWriter save = nullptr)
      : nameFormat_(nameFmt), nameExtension_(ext),
        operationRead_(read), operationSave_(save)
  {
  }
  explicit TagFileFormat(String nameFmt,
                         String ext,
                         FileParser read,
                         FileWriter save = nullptr)
      : nameFormat_(nameFmt), nameExtension_(ext),
      ptrParse_(read), ptrWrite_(save)
  {
  }
  //
  String getName() const { return nameFormat_; }
  bool hasName() const { return !nameFormat_.empty(); }
  //
  String getExtension() const { return nameExtension_; }
  bool hasExtension() const { return !nameExtension_.empty(); }
  bool isMaskEqual(const String &mask) const
  {
    return bool(0 == mask.compare(nameExtension_));
  }
  //
  bool hasRead() const { return bool(ptrParse_ != nullptr || operationRead_ != nullptr); }
  bool hasSave() const { return bool(ptrWrite_ != nullptr || operationSave_ != nullptr); }
  //
  bool isValid() const { return (hasRead() || hasSave()) && hasName(); }
  bool isNative() const { return hasRead() && hasSave() && hasName(); }
  bool isToLoad() const { return hasRead() && hasName(); }
  bool isToSave() const { return hasSave() && hasName(); }
  //
  bool implementRead(Host &host, Path path) const
  {
#ifdef QT_MSGBOX_DEBUG
    QMessageBox::information(nullptr,
                             QString("%1 format opening:").arg(this->getExtension().c_str()),
                             QString(path.c_str()));
#endif // !QT_MSGBOX_DEBUG
    if (!this->hasRead())
      return false;
    host.clearAll();
    if( (((ptrParse_ != nullptr) && (*ptrParse_)(path,host)) ||
         ((operationRead_ != nullptr) && (host.*operationRead_)(path))) )
    host.resetPath(path);
    return true;
  }
  //
  bool implementSave(Host &host, Path the_path) const
  {
    if (!this->hasSave() || (host.*operationSave_)(the_path))
      return false;
    // host.resetPath(the_path,true); // for the output path...
    return true;
  }
  //
  String getMask() const
  {
    String mask("*");
    mask += nameExtension_;
    return mask;
  }
  // bool
private:
  String nameFormat_ = String();
  String nameExtension_ = String();
  DataParser operationRead_ = nullptr;
  DataWriter operationSave_ = nullptr;
  // static external parse/write operations with higher priority
  FileParser ptrParse_ = nullptr;
  FileWriter ptrWrite_ = nullptr;
};

struct IsFormatValid
{
  template <class Host>
  bool operator()(const TagFileFormat<Host> &tag) const
  {
    return tag.isValid();
  }
};

struct IsFormatToLoad
{
  template <class Host>
  bool operator()(const TagFileFormat<Host> &tag) const
  {
    return tag.isToLoad();
  }
};

struct IsFormatToSave
{
  template <class Host>
  bool operator()(const TagFileFormat<Host> &tag) const
  {
    return tag.isToSave();
  }
};

struct IsFormatNative // both save and load
{
  template <class Host>
  bool operator()(const TagFileFormat<Host> &tag) const
  {
    return tag.isNative();
  }
};
//
#endif // !Tag_File_Format_h__
