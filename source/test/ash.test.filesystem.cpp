#include "ash.filesystem.h"
#include "ash.test.filesystem.h"


namespace Ash
{
	namespace Test
	{
		namespace FileSystem
		{
			namespace Path
			{
#if defined PLATFORM_WINDOWS
				static inline bool isEqual(const Ash::FileSystem::Path &left, const wchar_t *right)
				{
					Ash::FileSystem::Path::View leftValue = left.getView();
					return (leftValue.getLength() == wcslen(right)) && (wcsncmp(leftValue.at(0), right, leftValue.getLength()) == 0);
				}

				static Ash::Test::Assertion core()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path::supportRoot);
					TEST_IS_TRUE(Ash::FileSystem::Path::supportDrive);
					TEST_IS_TRUE(Ash::FileSystem::Path::supportNetworkShare);

					return {};
				}

				static Ash::Test::Assertion set()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.set(""), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set("."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set(u8"."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set(L"."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set("./"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set("./."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set("././"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set("././test"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test"));

					TEST_IS_EQ(path.set("././test/"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test"));
					TEST_IS_EQ(path.set("././test/."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test"));

					TEST_IS_EQ(path.set("././test/."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test"));

					TEST_IS_EQ(path.set("././test/./"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test"));

					TEST_IS_EQ(path.set("././test/./path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set("././test/./path/.."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path\\.."));

					TEST_IS_EQ(path.set(".test/.path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\.test\\.path"));

					TEST_IS_EQ(path.set(".", "."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					TEST_IS_EQ(path.set(".", ".", "test"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test"));

					TEST_IS_EQ(path.set(".", ".", "test", "."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test"));

					TEST_IS_EQ(path.set(".", ".", "test", ".", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", "test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", "test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", u8"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", u8"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", u8"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", L"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", L"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(".", L"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", "test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", "test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", u8"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", u8"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", u8"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", L"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", L"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(u8".", L"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", "test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", "test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", u8"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", u8"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", u8"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", L"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", L"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(L".", L"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					path.clear();

					TEST_IS_EQ(path.set(".", "invalid<"), Ash::FileSystem::Error::invalidPathComponent);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(".", "invalid>"), Ash::FileSystem::Error::invalidPathComponent);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(".", "invalid:"), Ash::FileSystem::Error::invalidPathComponent);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(".", "invalid\""), Ash::FileSystem::Error::invalidPathComponent);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(".", "invalid|"), Ash::FileSystem::Error::invalidPathComponent);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(".", "invalid?"), Ash::FileSystem::Error::invalidPathComponent);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(".", "invalid*"), Ash::FileSystem::Error::invalidPathComponent);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set("/"), Ash::FileSystem::Error::rootPathNotSupported);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set("/test/path"), Ash::FileSystem::Error::rootPathNotSupported);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set("d:"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:"));

					TEST_IS_EQ(path.set(u8"d:"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:"));

					TEST_IS_EQ(path.set(L"d:"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:"));

					TEST_IS_EQ(path.set("d:/"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:"));

					TEST_IS_EQ(path.set("d:/test"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:\\test"));

					path.clear();

					TEST_IS_EQ(path.set("d:test"), Ash::FileSystem::Error::invalidDrivePath);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set("//server/share/test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\test\\path"));

					TEST_IS_EQ(path.set("//server/share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.set("test"), Ash::Error::none);
					TEST_IS_EQ(path.set(path, "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set("path"), Ash::Error::none);
					TEST_IS_EQ(path.set("test", path), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set("d:/test"), Ash::Error::none);
					TEST_IS_EQ(path.set(path, "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:\\test\\path"));

					TEST_IS_EQ(path.set("path"), Ash::Error::none);
					TEST_IS_EQ(path.set("d:/test", path), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:\\test\\path"));

					TEST_IS_EQ(path.set("//server/share"), Ash::Error::none);
					TEST_IS_EQ(path.set(path, "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\path"));

					TEST_IS_EQ(path.set("path"), Ash::Error::none);
					TEST_IS_EQ(path.set("//server/share", path), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test"), "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test", "path"), "file"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\test\\path\\file"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("c:"), "test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:\\test\\path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path::fromDrive('c', "test"), "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:\\test\\path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("//server/share"), "test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\test\\path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path::fromNetworkShare("server", "share", "test"), "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\test\\path"));

					path.clear();

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test"), "/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test"), "c:/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test"), "//server/share/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("c:/test"), "c:/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("//server/share/test"), "c:/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					return {};
				}

				static Ash::Test::Assertion clear()
				{
					Ash::FileSystem::Path path = "./test/path";
					TEST_IS_TRUE(path.isValid());

					path.clear();
					TEST_IS_FALSE(path.isValid());
					TEST_IS_TRUE(isEqual(path, L""));

					return {};
				}

				static Ash::Test::Assertion construct()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(),                              L""));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(""),                            L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("."),                           L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8"."),                         L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L"."),                          L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./"),                          L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./."),                         L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././"),                        L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test"),                    L".\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/"),                   L".\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/."),                  L".\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/."),                  L".\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/./"),                 L".\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/./path"),             L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/./path/.."),          L".\\test\\path\\.."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".test/.path"),                 L".\\.test\\.path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "."),                      L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", ".", "test"),              L".\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", ".", "test", "."),         L".\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", ".", "test", ".", "path"), L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "test", "path"),           L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "test", u8"path"),         L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "test", L"path"),          L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", u8"test", "path"),         L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", u8"test", u8"path"),       L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", u8"test", L"path"),        L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", L"test", "path"),          L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", L"test", u8"path"),        L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", L"test", L"path"),         L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", "test", "path"),         L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", "test", u8"path"),       L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", "test", L"path"),        L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", u8"test", "path"),       L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", u8"test", u8"path"),     L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", u8"test", L"path"),      L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", L"test", "path"),        L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", L"test", u8"path"),      L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", L"test", L"path"),       L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", "test", "path"),          L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", "test", u8"path"),        L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", "test", L"path"),         L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", u8"test", "path"),        L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", u8"test", u8"path"),      L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", u8"test", L"path"),       L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", L"test", "path"),         L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", L"test", u8"path"),       L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", L"test", L"path"),        L".\\test\\path"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(".", "invalid<");

						TEST_FAIL("Ash::FileSystem::Error::invalidPathComponent exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidPathComponent);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(".", "invalid>");

						TEST_FAIL("Ash::FileSystem::Error::invalidPathComponent exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidPathComponent);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(".", "invalid:");

						TEST_FAIL("Ash::FileSystem::Error::invalidPathComponent exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidPathComponent);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(".", "invalid\"");

						TEST_FAIL("Ash::FileSystem::Error::invalidPathComponent exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidPathComponent);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(".", "invalid|");

						TEST_FAIL("Ash::FileSystem::Error::invalidPathComponent exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidPathComponent);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(".", "invalid?");

						TEST_FAIL("Ash::FileSystem::Error::invalidPathComponent exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidPathComponent);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(".", "invalid*");

						TEST_FAIL("Ash::FileSystem::Error::invalidPathComponent exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidPathComponent);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("/");

						TEST_FAIL("Ash::FileSystem::Error::rootPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::rootPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("/test/path");

						TEST_FAIL("Ash::FileSystem::Error::rootPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::rootPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("/test", "path");

						TEST_FAIL("Ash::FileSystem::Error::rootPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::rootPathNotSupported);
					}

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("d:"),          L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8"d:"),        L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L"d:"),         L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("d:/"),         L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("d:/", "test"), L"d:\\test"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("d:test");

						TEST_FAIL("Ash::FileSystem::Error::invalidDrivePath exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidDrivePath);
					}

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share/test/path"), L"\\\\server\\share\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share"), L"\\\\server\\share"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("///server/share");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkPath exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkPath);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("//server/.");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("//server/..");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("//server");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("//./share");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("//../share");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("//");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path("test"), "path"),                                      L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path("test", "path"), "file"),                              L".\\test\\path\\file"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path("c:"), "test/path"),                                   L"c:\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path::fromDrive('c', "test"), "path"),                      L"c:\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path("//server/share"), "test/path"),                       L"\\\\server\\share\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path::fromNetworkShare("server", "share", "test"), "path"), L"\\\\server\\share\\test\\path"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(Ash::FileSystem::Path("test"), "/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("test", "c:/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("test", "//server/share/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("c:/test", "c:/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path("//server/share/test", "c:/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					return {};
				}

				static Ash::Test::Assertion setRoot()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.setRoot(""), Ash::FileSystem::Error::rootPathNotSupported);
					TEST_IS_TRUE(isEqual(path, L""));

					return {};
				}

				static Ash::Test::Assertion fromRoot()
				{
					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromRoot("");

						TEST_FAIL("Ash::FileSystem::Error::rootPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::rootPathNotSupported);
					}

					return {};
				}

				static Ash::Test::Assertion setDrive()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.setDrive('A'), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"A:"));

					TEST_IS_EQ(path.setDrive('Z'), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"Z:"));

					TEST_IS_EQ(path.setDrive('a'), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"a:"));

					TEST_IS_EQ(path.setDrive('z'), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"z:"));

					TEST_IS_EQ(path.setDrive('C', "test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"C:\\test\\path"));

					TEST_IS_EQ(path.setDrive('c', "./test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:\\test\\path"));

					TEST_IS_EQ(path.setDrive('d', Ash::FileSystem::Path("folder/file.txt")), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"d:\\folder\\file.txt"));

					path.clear();

					TEST_IS_EQ(path.setDrive('#'), Ash::FileSystem::Error::invalidDrive);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setDrive('C', "/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setDrive('C', "c:/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setDrive('C', "//server/share/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setDrive('d', Ash::FileSystem::Path("c:/test/path")), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setDrive('d', Ash::FileSystem::Path("//server/share/path")), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					return {};
				}

				static Ash::Test::Assertion fromDrive()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('A'),                                           L"A:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('Z'),                                           L"Z:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('a'),                                           L"a:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('z'),                                           L"z:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('C', "test/path"),                              L"C:\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('c', "./test/path"),                            L"c:\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('d', Ash::FileSystem::Path("folder/file.txt")), L"d:\\folder\\file.txt"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('#');

						TEST_FAIL("Ash::FileSystem::Error::invalidDrive exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidDrive);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('C', "/test/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('C', "c:/test/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('C', "//server/share/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('d', Ash::FileSystem::Path("c:/test/path"));

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('d', Ash::FileSystem::Path("//server/share/path"));

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					return {};
				}

				static Ash::Test::Assertion setNetworkShare()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.setNetworkShare("server", "share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare("server", u8"share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare("server", L"share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare(u8"server", "share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare(u8"server", u8"share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare(u8"server", L"share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare(L"server", "share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare(L"server", u8"share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare(L"server", L"share"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					TEST_IS_EQ(path.setNetworkShare("server", "share", "test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\test\\path"));

					TEST_IS_EQ(path.setNetworkShare("server", "share", "./test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\test\\path"));

					TEST_IS_EQ(path.setNetworkShare("server", "share", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\test\\path"));

					TEST_IS_EQ(path.setNetworkShare("server", "share", Ash::FileSystem::Path("folder/file.txt")), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\folder\\file.txt"));

					path.clear();

					TEST_IS_EQ(path.setNetworkShare("server", "share", "/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "share", "c:/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "share", "//server/share/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "share", Ash::FileSystem::Path("c:/folder/file.txt")), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "share", Ash::FileSystem::Path("//server/folder/file.text")), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", ".", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "..", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid<", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid>", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid:", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid\"", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid/", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid\\", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid|", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid?", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("server", "invalid*", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare(".", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("..", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid<", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid>", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid:", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid\"", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid/", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid\\", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid|", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid?", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					TEST_IS_EQ(path.setNetworkShare("invalid*", "share", "test/path"), Ash::FileSystem::Error::invalidNetworkName);
					TEST_IS_TRUE(isEqual(path, L""));

					return {};
				}

				static Ash::Test::Assertion fromNetworkShare()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share"),                                           L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", u8"share"),                                         L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", L"share"),                                          L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", "share"),                                         L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", u8"share"),                                       L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", L"share"),                                        L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", "share"),                                          L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", u8"share"),                                        L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", L"share"),                                         L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", "test/path"),                              L"\\\\server\\share\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", "./test/path"),                            L"\\\\server\\share\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", "test", "path"),                           L"\\\\server\\share\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", Ash::FileSystem::Path("folder/file.txt")), L"\\\\server\\share\\folder\\file.txt"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "share", "/test/path");

						TEST_FAIL("Ash::FileSystem::Error::rootPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "share", "c:/test/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "share", "//server/share/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "share", Ash::FileSystem::Path("c:/folder/file.txt"));

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "share", Ash::FileSystem::Path("//server/folder/file.text"));

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", ".", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "..", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid<", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid>", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid:", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid\"", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid/", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid\\", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid|", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid?", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "invalid*", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare(".", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("..", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid<", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid>", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid:", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid\"", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid/", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid\\", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid|", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid?", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("invalid*", "share", "test/path");

						TEST_FAIL("Ash::FileSystem::Error::invalidNetworkName exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::invalidNetworkName);
					}

					return {};
				}

				Ash::Test::Assertion setCurrentDirectory()
				{
					Ash::FileSystem::Path currentDirectory;

					TEST_IS_EQ(currentDirectory.setCurrentDirectory(), Ash::Error::none);
					TEST_IS_NOT_NULL(currentDirectory.getView().at(0));
					TEST_IS_NOT_ZERO(*currentDirectory.getView().at(0));

					Ash::FileSystem::Path directory1(currentDirectory, "test/path");
					Ash::FileSystem::Path directory2;
					TEST_IS_EQ(directory2.setCurrentDirectory("test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(directory1, directory2));
					Ash::FileSystem::Path directory3;
					TEST_IS_EQ(directory3.setCurrentDirectory(Ash::FileSystem::Path("test/path")), Ash::Error::none);
					TEST_IS_TRUE(isEqual(directory1, directory3));

					currentDirectory.clear();

					TEST_IS_EQ(currentDirectory.setCurrentDirectory("/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(currentDirectory, L""));

					TEST_IS_EQ(currentDirectory.setCurrentDirectory("c:/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(currentDirectory, L""));

					TEST_IS_EQ(currentDirectory.setCurrentDirectory("//server/share/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(currentDirectory, L""));

					TEST_IS_EQ(currentDirectory.setCurrentDirectory(Ash::FileSystem::Path("c:/test/path")), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(currentDirectory, L""));

					TEST_IS_EQ(currentDirectory.setCurrentDirectory(Ash::FileSystem::Path("//server/share/path")), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(currentDirectory, L""));

					return {};
				}

				Ash::Test::Assertion fromCurrentDirectory()
				{
					Ash::FileSystem::Path currentDirectory = Ash::FileSystem::Path::fromCurrentDirectory();
					TEST_IS_NOT_NULL(currentDirectory.getView().at(0));
					TEST_IS_NOT_ZERO(*currentDirectory.getView().at(0));

					Ash::FileSystem::Path directory1(currentDirectory, "test/path");
					Ash::FileSystem::Path directory2 = Ash::FileSystem::Path::fromCurrentDirectory("test/path");
					TEST_IS_TRUE(isEqual(directory1, directory2));
					Ash::FileSystem::Path directory3 = Ash::FileSystem::Path::fromCurrentDirectory(Ash::FileSystem::Path("test/path"));
					TEST_IS_TRUE(isEqual(directory1, directory3));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromCurrentDirectory("/test/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromCurrentDirectory("c:/test/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromCurrentDirectory("//server/share/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromCurrentDirectory(Ash::FileSystem::Path("c:/test/path"));

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromCurrentDirectory(Ash::FileSystem::Path("//server/share/path"));

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					return {};
				}

				Ash::Test::Assertion isValid()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromDrive('c').isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromNetworkShare("server", "share").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory().isValid());

					return {};
				}

				Ash::Test::Assertion isRelative()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isRelative());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromDrive('c').isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromNetworkShare("server", "share").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromCurrentDirectory().isRelative());

					return {};
				}

				Ash::Test::Assertion isAbsolute()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path("").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromDrive('c').isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromNetworkShare("server", "share").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory().isAbsolute());

					return {};
				}

				Ash::Test::Assertion getType()
				{
					TEST_IS_EQ(Ash::FileSystem::Path().getType(),                 Ash::FileSystem::Path::Type::invalid);
					TEST_IS_EQ(Ash::FileSystem::Path(".").getType(),              Ash::FileSystem::Path::Type::relativeContained);
					TEST_IS_EQ(Ash::FileSystem::Path("..").getType(),             Ash::FileSystem::Path::Type::relativeUncontained);
					TEST_IS_EQ(Ash::FileSystem::Path("test").getType(),           Ash::FileSystem::Path::Type::relativeContained);
					TEST_IS_EQ(Ash::FileSystem::Path("test/..").getType(),        Ash::FileSystem::Path::Type::relativeContained);
					TEST_IS_EQ(Ash::FileSystem::Path("test/../..").getType(),     Ash::FileSystem::Path::Type::relativeUncontained);
					TEST_IS_EQ(Ash::FileSystem::Path("c:").getType(),             Ash::FileSystem::Path::Type::absoluteDrive);
					TEST_IS_EQ(Ash::FileSystem::Path("//server/share").getType(), Ash::FileSystem::Path::Type::absoluteNetworkShare);

					return {};
				}

				Ash::Test::Assertion reduce()
				{
					Ash::FileSystem::Path reducedPath;

					TEST_IS_EQ(Ash::FileSystem::Path().reduce(reducedPath), Ash::FileSystem::Error::invalidPath);
					TEST_IS_TRUE(isEqual(reducedPath, L""));

					TEST_IS_EQ(Ash::FileSystem::Path(".").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"."));

					TEST_IS_EQ(Ash::FileSystem::Path("./..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L".\\.."));

					TEST_IS_EQ(Ash::FileSystem::Path("./test/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"."));

					TEST_IS_EQ(Ash::FileSystem::Path("./test/../..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L".\\.."));

					TEST_IS_EQ(Ash::FileSystem::Path("./../test/../../path").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L".\\..\\..\\path"));

					TEST_IS_EQ(Ash::FileSystem::Path("c:").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"c:"));

					TEST_IS_EQ(Ash::FileSystem::Path("c:/test").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"c:\\test"));

					TEST_IS_EQ(Ash::FileSystem::Path("c:/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"c:"));

					TEST_IS_EQ(Ash::FileSystem::Path("c:/test/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"c:"));

					TEST_IS_EQ(Ash::FileSystem::Path("c:/test/../..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"c:"));

					TEST_IS_EQ(Ash::FileSystem::Path("c:/../test/../../path").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"c:\\path"));

					TEST_IS_EQ(Ash::FileSystem::Path("//server/share").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"\\\\server\\share"));

					TEST_IS_EQ(Ash::FileSystem::Path("//server/share/test").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"\\\\server\\share\\test"));

					TEST_IS_EQ(Ash::FileSystem::Path("//server/share/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"\\\\server\\share"));

					TEST_IS_EQ(Ash::FileSystem::Path("//server/share/test/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"\\\\server\\share"));

					TEST_IS_EQ(Ash::FileSystem::Path("//server/share/test/../..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"\\\\server\\share"));

					TEST_IS_EQ(Ash::FileSystem::Path("//server/share/../test/../../path").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, L"\\\\server\\share\\path"));

					Ash::FileSystem::Path path = "./../test/../../path";
					TEST_IS_EQ(path.reduce(path), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\..\\..\\path"));

					path.clear();
					TEST_IS_EQ(path.reduce(), Ash::FileSystem::Error::invalidPath);
					TEST_IS_TRUE(isEqual(path, L""));

					path = ".";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					path = "./..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\.."));

					path = "./test/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"."));

					path = "./test/../..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\.."));

					path = "./../test/../../path";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L".\\..\\..\\path"));

					path = "c:";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:"));

					path = "c:/test";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:\\test"));

					path = "c:/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:"));

					path = "c:/test/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:"));

					path = "c:/test/../..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:"));

					path = "c:/../test/../../path";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"c:\\path"));

					path = "//server/share";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					path = "//server/share/test";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\test"));

					path = "//server/share/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					path = "//server/share/test/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					path = "//server/share/test/../..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share"));

					path = "//server/share/../test/../../path";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, L"\\\\server\\share\\path"));

					return {};
				}

				Ash::Test::Assertion getLastComponent()
				{
					static struct
					{
						const char *path;
						const char *lastComponent;
					} testCases[] =
					{
						{ ".",                        "."                 },
						{ "..",                       ".."                },
						{ "../",                      ".."                },
						{ "../.",                     ".."                },
						{ "test",                     "test"              },
						{ "test/",                    "test"              },
						{ "test/.",                   "test"              },
						{ "./test",                   "test"              },
						{ "./test/",                  "test"              },
						{ "./test/.",                 "test"              },
						{ "../test",                  "test"              },
						{ "../test/",                 "test"              },
						{ "../test/.",                "test"              },
						{ "./test/..",                ".."                },
						{ "./test/../",               ".."                },
						{ "./test/../.",              ".."                },

						{ "c:",                       "c:"                },
						{ "c:/",                      "c:"                },
						{ "c:/.",                     "c:"                },
						{ "c:/..",                    ".."                },
						{ "c:/../",                   ".."                },
						{ "c:/../.",                  ".."                },
						{ "c:/test",                  "test"              },
						{ "c:/test/",                 "test"              },
						{ "c:/test/.",                "test"              },
						{ "c:/test/..",               ".."                },
						{ "c:/test/../",              ".."                },
						{ "c:/test/../.",             ".."                },

						{ "//server/share",           "\\\\server\\share" },
						{ "//server/share/",          "\\\\server\\share" },
						{ "//server/share/.",         "\\\\server\\share" },
						{ "//server/share/..",        ".."                },
						{ "//server/share/../",       ".."                },
						{ "//server/share/../.",      ".."                },
						{ "//server/share/test",      "test"              },
						{ "//server/share/test/",     "test"              },
						{ "//server/share/test/.",    "test"              },
						{ "//server/share/test/..",   ".."                },
						{ "//server/share/test/../",  ".."                },
						{ "//server/share/test/../.", ".."                }
					};
					Ash::Utf8::String<> component;

					TEST_IS_EQ(Ash::FileSystem::Path().getLastComponent(component), Ash::FileSystem::Error::invalidPath);

					for (size_t n = 0; n < sizeof(testCases) / sizeof(testCases[0]); n++)
					{
						TEST_IS_EQ(Ash::FileSystem::Path(testCases[n].path).getLastComponent(component), Ash::Error::none);
						TEST_IS_EQ(component.getLength(), strlen(testCases[n].lastComponent));
						TEST_IS_ZERO(memcmp(component.at(0), testCases[n].lastComponent, component.getLength()));
					}

					Ash::Wide::String<> wideComponent;
					TEST_IS_EQ(Ash::FileSystem::Path("./test").getLastComponent(wideComponent), Ash::Error::none);
					TEST_IS_EQ(wideComponent.getLength(), wcslen(L"test"));
					TEST_IS_ZERO(memcmp(wideComponent.at(0), L"test", wideComponent.getLength()));

					Ash::Utf8::String<> utf8Component;
					TEST_IS_EQ(Ash::FileSystem::Path("./test").getLastComponent(utf8Component), Ash::Error::none);
					TEST_IS_EQ(utf8Component.getLength(), strlen("test"));
					TEST_IS_ZERO(memcmp(utf8Component.at(0), "test", utf8Component.getLength()));

					Ash::Ascii::String<> asciiComponent;
					TEST_IS_EQ(Ash::FileSystem::Path("./test").getLastComponent(asciiComponent), Ash::Error::none);
					TEST_IS_EQ(asciiComponent.getLength(), strlen("test"));
					TEST_IS_ZERO(memcmp(asciiComponent.at(0), "test", asciiComponent.getLength()));

					return {};
				}
#elif defined PLATFORM_LINUX
				static inline bool isEqual(const Ash::FileSystem::Path &left, const Ash::FileSystem::Path::Encoding::Code *right)
				{
					Ash::FileSystem::Path::View leftValue = left.getView();
					return (leftValue.getLength() == strlen((const char *)right)) && (strncmp((const char *)leftValue.at(0), (const char *)right, leftValue.getLength()) == 0);
				}

				static Ash::Test::Assertion core()
				{
					TEST_IS_TRUE(Ash::FileSystem::Path::supportRoot);
					TEST_IS_FALSE(Ash::FileSystem::Path::supportDrive);
					TEST_IS_FALSE(Ash::FileSystem::Path::supportNetworkShare);

					return {};
				}

				static Ash::Test::Assertion set()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.set(""), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set("."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set(u8"."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set(L"."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set("./"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set("./."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set("././"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set("././test"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test"));

					TEST_IS_EQ(path.set("././test/"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test"));
					TEST_IS_EQ(path.set("././test/."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test"));

					TEST_IS_EQ(path.set("././test/."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test"));

					TEST_IS_EQ(path.set("././test/./"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test"));

					TEST_IS_EQ(path.set("././test/./path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set("././test/./path/.."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path/.."));

					TEST_IS_EQ(path.set(".test/.path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./.test/.path"));

					TEST_IS_EQ(path.set(".", "."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					TEST_IS_EQ(path.set(".", ".", "test"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test"));

					TEST_IS_EQ(path.set(".", ".", "test", "."), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test"));

					TEST_IS_EQ(path.set(".", ".", "test", ".", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", "test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", "test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", u8"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", u8"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", u8"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", L"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", L"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(".", L"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", "test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", "test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", u8"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", u8"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", u8"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", L"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", L"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(u8".", L"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", "test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", "test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", u8"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", u8"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", u8"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", L"test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", L"test", u8"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(L".", L"test", L"path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set("/"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					TEST_IS_EQ(path.set(u8"/"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					TEST_IS_EQ(path.set(L"/"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					TEST_IS_EQ(path.set("/test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					TEST_IS_EQ(path.set("/test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					TEST_IS_EQ(path.set("/", "test", "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					TEST_IS_EQ(path.set("test"), Ash::Error::none);
					TEST_IS_EQ(path.set(path, "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set("path"), Ash::Error::none);
					TEST_IS_EQ(path.set("test", path), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set("/test"), Ash::Error::none);
					TEST_IS_EQ(path.set(path, "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					TEST_IS_EQ(path.set("path"), Ash::Error::none);
					TEST_IS_EQ(path.set("/test", path), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test"), "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test", "path"), "file"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./test/path/file"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("/"), "test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path::fromRoot("test"), "path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					path.clear();

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("test"), "/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.set(Ash::FileSystem::Path("/test"), "/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, u8""));

					return {};
				}

				static Ash::Test::Assertion clear()
				{
					Ash::FileSystem::Path path = "./test/path";
					TEST_IS_TRUE(path.isValid());

					path.clear();
					TEST_IS_FALSE(path.isValid());
					TEST_IS_TRUE(isEqual(path, u8""));

					return {};
				}

				static Ash::Test::Assertion construct()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(),                              u8""));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(""),                            u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("."),                           u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8"."),                         u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L"."),                          u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./"),                          u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./."),                         u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././"),                        u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test"),                    u8"./test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/"),                   u8"./test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/."),                  u8"./test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/."),                  u8"./test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/./"),                 u8"./test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/./path"),             u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("././test/./path/.."),          u8"./test/path/.."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".test/.path"),                 u8"./.test/.path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "."),                      u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", ".", "test"),              u8"./test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", ".", "test", "."),         u8"./test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", ".", "test", ".", "path"), u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "test", "path"),           u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "test", u8"path"),         u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "test", L"path"),          u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", u8"test", "path"),         u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", u8"test", u8"path"),       u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", u8"test", L"path"),        u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", L"test", "path"),          u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", L"test", u8"path"),        u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", L"test", L"path"),         u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", "test", "path"),         u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", "test", u8"path"),       u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", "test", L"path"),        u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", u8"test", "path"),       u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", u8"test", u8"path"),     u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", u8"test", L"path"),      u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", L"test", "path"),        u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", L"test", u8"path"),      u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8".", L"test", L"path"),       u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", "test", "path"),          u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", "test", u8"path"),        u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", "test", L"path"),         u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", u8"test", "path"),        u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", u8"test", u8"path"),      u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", u8"test", L"path"),       u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", L"test", "path"),         u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", L"test", u8"path"),       u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L".", L"test", L"path"),        u8"./test/path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/"),                           u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/test/path"),                  u8"/test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/test", "path"),               u8"/test/path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path("test"), "path"),           u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path("test", "path"), "file"),   u8"./test/path/file"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path("/"), "test/path"),         u8"/test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(Ash::FileSystem::Path::fromRoot("test"), "path"), u8"/test/path"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path(Ash::FileSystem::Path("test"), "/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					return {};
				}

				static Ash::Test::Assertion setRoot()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.setRoot(""), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					TEST_IS_EQ(path.setRoot("test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test/path"));

					path.clear();

					TEST_IS_EQ(path.setRoot("/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(path, u8""));

					return {};
				}

				static Ash::Test::Assertion fromRoot()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromRoot(""), u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromRoot("test/path"), u8"/test/path"));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromRoot("/test/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					return {};
				}

				static Ash::Test::Assertion setDrive()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.setDrive('A'), Ash::FileSystem::Error::drivePathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					return {};
				}

				static Ash::Test::Assertion fromDrive()
				{
					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('A');

						TEST_FAIL("Ash::FileSystem::Error::drivePathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::drivePathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromDrive('C', "test/path");

						TEST_FAIL("Ash::FileSystem::Error::drivePathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::drivePathNotSupported);
					}

					return {};
				}

				static Ash::Test::Assertion setNetworkShare()
				{
					Ash::FileSystem::Path path;

					TEST_IS_EQ(path.setNetworkShare("server", "share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare("server", u8"share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare("server", L"share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare(u8"server", "share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare(u8"server", u8"share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare(u8"server", L"share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare(L"server", "share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare(L"server", u8"share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					TEST_IS_EQ(path.setNetworkShare(L"server", L"share"), Ash::FileSystem::Error::networkPathNotSupported);
					TEST_IS_TRUE(isEqual(path, u8""));

					return {};
				}

				static Ash::Test::Assertion fromNetworkShare()
				{
					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", "share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", u8"share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare("server", L"share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare(u8"server", "share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare(u8"server", u8"share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare(u8"server", L"share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare(L"server", "share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare(L"server", u8"share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromNetworkShare(L"server", L"share");

						TEST_FAIL("Ash::FileSystem::Error::networkPathNotSupported exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::networkPathNotSupported);
					}

					return {};
				}

				Ash::Test::Assertion setCurrentDirectory()
				{
					Ash::FileSystem::Path currentDirectory;

					TEST_IS_EQ(currentDirectory.setCurrentDirectory(), Ash::Error::none);
					TEST_IS_NOT_NULL(currentDirectory.getView().at(0));
					TEST_IS_NOT_ZERO(*currentDirectory.getView().at(0));

					Ash::FileSystem::Path directory1(currentDirectory, "test/path");
					Ash::FileSystem::Path directory2;
					TEST_IS_EQ(directory2.setCurrentDirectory("test/path"), Ash::Error::none);
					TEST_IS_TRUE(isEqual(directory1, directory2));
					Ash::FileSystem::Path directory3;
					TEST_IS_EQ(directory3.setCurrentDirectory(Ash::FileSystem::Path("test/path")), Ash::Error::none);
					TEST_IS_TRUE(isEqual(directory1, directory3));

					currentDirectory.clear();

					TEST_IS_EQ(currentDirectory.setCurrentDirectory("/test/path"), Ash::FileSystem::Error::pathNotRelative);
					TEST_IS_TRUE(isEqual(currentDirectory, u8""));

					return {};
				}

				Ash::Test::Assertion fromCurrentDirectory()
				{
					Ash::FileSystem::Path currentDirectory = Ash::FileSystem::Path::fromCurrentDirectory();
					TEST_IS_NOT_NULL(currentDirectory.getView().at(0));
					TEST_IS_NOT_ZERO(*currentDirectory.getView().at(0));

					Ash::FileSystem::Path directory1(currentDirectory, "test/path");
					Ash::FileSystem::Path directory2 = Ash::FileSystem::Path::fromCurrentDirectory("test/path");
					TEST_IS_TRUE(isEqual(directory1, directory2));
					Ash::FileSystem::Path directory3 = Ash::FileSystem::Path::fromCurrentDirectory(Ash::FileSystem::Path("test/path"));
					TEST_IS_TRUE(isEqual(directory1, directory3));

					try
					{
						Ash::FileSystem::Path path = Ash::FileSystem::Path::fromCurrentDirectory("/test/path");

						TEST_FAIL("Ash::FileSystem::Error::pathNotRelative exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::FileSystem::Error::pathNotRelative);
					}

					return {};
				}

				Ash::Test::Assertion isValid()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromRoot("").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory("").isValid());

					return {};
				}

				Ash::Test::Assertion isRelative()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isRelative());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromRoot("").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromCurrentDirectory("").isRelative());

					return {};
				}

				Ash::Test::Assertion isAbsolute()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path("").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromRoot("").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory("").isAbsolute());

					return {};
				}

				Ash::Test::Assertion getType()
				{
					TEST_IS_EQ(Ash::FileSystem::Path().getType(),             Ash::FileSystem::Path::Type::invalid);
					TEST_IS_EQ(Ash::FileSystem::Path(".").getType(),          Ash::FileSystem::Path::Type::relativeContained);
					TEST_IS_EQ(Ash::FileSystem::Path("..").getType(),         Ash::FileSystem::Path::Type::relativeUncontained);
					TEST_IS_EQ(Ash::FileSystem::Path("test").getType(),       Ash::FileSystem::Path::Type::relativeContained);
					TEST_IS_EQ(Ash::FileSystem::Path("test/..").getType(),    Ash::FileSystem::Path::Type::relativeContained);
					TEST_IS_EQ(Ash::FileSystem::Path("test/../..").getType(), Ash::FileSystem::Path::Type::relativeUncontained);
					TEST_IS_EQ(Ash::FileSystem::Path("/").getType(),          Ash::FileSystem::Path::Type::absoluteRoot);

					return {};
				}

				Ash::Test::Assertion reduce()
				{
					Ash::FileSystem::Path reducedPath;

					TEST_IS_EQ(Ash::FileSystem::Path().reduce(reducedPath), Ash::FileSystem::Error::invalidPath);
					TEST_IS_TRUE(isEqual(reducedPath, u8""));

					TEST_IS_EQ(Ash::FileSystem::Path(".").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"."));

					TEST_IS_EQ(Ash::FileSystem::Path("./..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"./.."));

					TEST_IS_EQ(Ash::FileSystem::Path("./test/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"."));

					TEST_IS_EQ(Ash::FileSystem::Path("./test/../..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"./.."));

					TEST_IS_EQ(Ash::FileSystem::Path("./../test/../../path").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"./../../path"));

					TEST_IS_EQ(Ash::FileSystem::Path("/").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"/"));

					TEST_IS_EQ(Ash::FileSystem::Path("/test").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"/test"));

					TEST_IS_EQ(Ash::FileSystem::Path("/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"/"));

					TEST_IS_EQ(Ash::FileSystem::Path("/test/..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"/"));

					TEST_IS_EQ(Ash::FileSystem::Path("/test/../..").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"/"));

					TEST_IS_EQ(Ash::FileSystem::Path("/../test/../../path").reduce(reducedPath), Ash::Error::none);
					TEST_IS_TRUE(isEqual(reducedPath, u8"/path"));

					Ash::FileSystem::Path path = "./../test/../../path";
					TEST_IS_EQ(path.reduce(path), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./../../path"));

					path.clear();
					TEST_IS_EQ(path.reduce(), Ash::FileSystem::Error::invalidPath);
					TEST_IS_TRUE(isEqual(path, u8""));

					path = ".";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					path = "./..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./.."));

					path = "./test/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"."));

					path = "./test/../..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./.."));

					path = "./../test/../../path";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"./../../path"));

					path = "/";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					path = "/test";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/test"));

					path = "/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					path = "/test/..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					path = "/test/../..";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/"));

					path = "/../test/../../path";
					TEST_IS_EQ(path.reduce(), Ash::Error::none);
					TEST_IS_TRUE(isEqual(path, u8"/path"));

					return {};
				}

				Ash::Test::Assertion getLastComponent()
				{
					static struct
					{
						const char *path;
						const char *lastComponent;
					} testCases[] =
					{
						{ ".",           "."    },
						{ "..",          ".."   },
						{ "../",         ".."   },
						{ "../.",        ".."   },
						{ "test",        "test" },
						{ "test/",       "test" },
						{ "test/.",      "test" },
						{ "./test",      "test" },
						{ "./test/",     "test" },
						{ "./test/.",    "test" },
						{ "../test",     "test" },
						{ "../test/",    "test" },
						{ "../test/.",   "test" },
						{ "./test/..",   ".."   },
						{ "./test/../",  ".."   },
						{ "./test/../.", ".."   },

						{ "/",           "/"    },
						{ "/.",          "/"    },
						{ "/..",         ".."   },
						{ "/../",        ".."   },
						{ "/../.",       ".."   },
						{ "/test",       "test" },
						{ "/test/",      "test" },
						{ "/test/.",     "test" },
						{ "/test/..",    ".."   },
						{ "/test/../",   ".."   },
						{ "/test/../.",  ".."   }
					};
					Ash::Utf8::String<> component;

					TEST_IS_EQ(Ash::FileSystem::Path().getLastComponent(component), Ash::FileSystem::Error::invalidPath);

					for (size_t n = 0; n < sizeof(testCases) / sizeof(testCases[0]); n++)
					{
						TEST_IS_EQ(Ash::FileSystem::Path(testCases[n].path).getLastComponent(component), Ash::Error::none);
						TEST_IS_EQ(component.getLength(), strlen(testCases[n].lastComponent));
						TEST_IS_ZERO(memcmp(component.at(0), testCases[n].lastComponent, component.getLength()));
					}

					Ash::Wide::String<> wideComponent;
					TEST_IS_EQ(Ash::FileSystem::Path("./test").getLastComponent(wideComponent), Ash::Error::none);
					TEST_IS_EQ(wideComponent.getLength(), wcslen(L"test"));
					TEST_IS_ZERO(memcmp(wideComponent.at(0), L"test", wideComponent.getLength()));

					Ash::Utf8::String<> utf8Component;
					TEST_IS_EQ(Ash::FileSystem::Path("./test").getLastComponent(utf8Component), Ash::Error::none);
					TEST_IS_EQ(utf8Component.getLength(), strlen("test"));
					TEST_IS_ZERO(memcmp(utf8Component.at(0), "test", utf8Component.getLength()));

					Ash::Ascii::String<> asciiComponent;
					TEST_IS_EQ(Ash::FileSystem::Path("./test").getLastComponent(asciiComponent), Ash::Error::none);
					TEST_IS_EQ(asciiComponent.getLength(), strlen("test"));
					TEST_IS_ZERO(memcmp(asciiComponent.at(0), "test", asciiComponent.getLength()));

					return {};
				}
#endif
			}
		}

		TEST_UNIT
		(
			testFileSystem,

			TEST_CASE(Ash::Test::FileSystem::Path::core),
			TEST_CASE(Ash::Test::FileSystem::Path::set),
			TEST_CASE(Ash::Test::FileSystem::Path::clear),
			TEST_CASE(Ash::Test::FileSystem::Path::construct),
			TEST_CASE(Ash::Test::FileSystem::Path::setRoot),
			TEST_CASE(Ash::Test::FileSystem::Path::fromRoot),
			TEST_CASE(Ash::Test::FileSystem::Path::setDrive),
			TEST_CASE(Ash::Test::FileSystem::Path::fromDrive),
			TEST_CASE(Ash::Test::FileSystem::Path::setNetworkShare),
			TEST_CASE(Ash::Test::FileSystem::Path::fromNetworkShare),
			TEST_CASE(Ash::Test::FileSystem::Path::setCurrentDirectory),
			TEST_CASE(Ash::Test::FileSystem::Path::fromCurrentDirectory),
			TEST_CASE(Ash::Test::FileSystem::Path::isValid),
			TEST_CASE(Ash::Test::FileSystem::Path::isRelative),
			TEST_CASE(Ash::Test::FileSystem::Path::isAbsolute),
			TEST_CASE(Ash::Test::FileSystem::Path::getType),
			TEST_CASE(Ash::Test::FileSystem::Path::reduce),
			TEST_CASE(Ash::Test::FileSystem::Path::getLastComponent)
		);
	}
}
