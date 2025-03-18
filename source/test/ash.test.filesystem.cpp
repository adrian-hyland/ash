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
#if defined __WIN32__
				static inline bool isEqual(const Ash::FileSystem::Path &left, const wchar_t *right)
				{
					Ash::FileSystem::Path::View leftValue = left.getValue();
					return (leftValue.getLength() == wcslen(right)) && (wcsncmp(leftValue.at(0), right, leftValue.getLength()) == 0);
				}

				static Ash::Test::Assertion core()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path::supportRoot);
					TEST_IS_TRUE(Ash::FileSystem::Path::supportDrive);
					TEST_IS_TRUE(Ash::FileSystem::Path::supportNetworkShare);

					return {};
				}

				static Ash::Test::Assertion relative()
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

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid<"),               L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid>"),               L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid:"),               L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid\""),              L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid/"),               L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid\\"),              L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid|"),               L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid?"),               L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid*"),               L""));

					return {};
				}

				static Ash::Test::Assertion fromRoot()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromRoot(""), L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/"),          L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/test/path"), L""));

					return {};
				}

				static Ash::Test::Assertion fromDrive()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('A', ""),           L"A:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('Z', ""),           L"Z:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('a', ""),           L"a:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('z', ""),           L"z:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('#', ""),           L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('C', "test/path"),  L"C:\\test\\path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('C', "/test/path"), L""));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("d:"),                         L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(u8"d:"),                       L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(L"d:"),                        L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("d:/"),                        L"d:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("d:/test"),                    L"d:\\test"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("d:test"),                     L""));

					return {};
				}

				static Ash::Test::Assertion fromNetworkShare()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", ""),              L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", u8"share", ""),            L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", L"share", ""),             L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", "share", ""),            L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", u8"share", ""),          L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", L"share", ""),           L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", "share", ""),             L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", u8"share", ""),           L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", L"share", ""),            L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", "test/path"),     L"\\\\server\\share\\test\\path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", "/test/path"),    L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "", "test/path"),          L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", ".", "test/path"),         L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "..", "test/path"),        L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid<", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid>", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid:", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid\"", "test/path"), L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid/", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid\\", "test/path"), L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid|", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid?", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "invalid*", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("", "share", "test/path"),           L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(".", "share", "test/path"),          L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("..", "share", "test/path"),         L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid<", "share", "test/path"),   L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid>", "share", "test/path"),   L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid:", "share", "test/path"),   L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid\"", "share", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid/", "share", "test/path"),   L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid\\", "share", "test/path"),  L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid|", "share", "test/path"),   L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid?", "share", "test/path"),   L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("invalid*", "share", "test/path"),   L""));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share/test/path"),                           L"\\\\server\\share\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share"),                                     L"\\\\server\\share"));
	
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("///server/share"),                                    L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/."),                                         L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/.."),                                        L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server"),                                           L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//./share"),                                          L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//../share"),                                         L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//"),                                                 L""));

					return {};
				}

				Ash::Test::Assertion fromBase()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("test", "path"),                                                             L".\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase(Ash::FileSystem::Path("test", "path"), "file"),                              L".\\test\\path\\file"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("c:", "test/path"),                                                          L"c:\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase(Ash::FileSystem::Path::fromDrive('c', "test"), "path"),                      L"c:\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("//server/share", "test/path"),                                              L"\\\\server\\share\\test\\path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase(Ash::FileSystem::Path::fromNetworkShare("server", "share", "test"), "path"), L"\\\\server\\share\\test\\path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("test", "/path"),                                                            L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("c:/test", "/path"),                                                         L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("//server/share/test", "/path"),                                             L""));

					return {};
				}

				Ash::Test::Assertion fromCurrentDirectory()
				{
					Ash::FileSystem::Path currentDirectory = Ash::FileSystem::Path::fromCurrentDirectory("");
					TEST_IS_NOT_NULL(currentDirectory.getValue().at(0));
					TEST_IS_NOT_ZERO(*currentDirectory.getValue().at(0));

					Ash::FileSystem::Path directory1 = Ash::FileSystem::Path::fromCurrentDirectory("test/path");
					Ash::FileSystem::Path directory2 = Ash::FileSystem::Path::fromBase(currentDirectory, "test/path");
					TEST_IS_TRUE(isEqual(directory1, directory2));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromCurrentDirectory("/test/path"), L""));

					return {};
				}

				Ash::Test::Assertion isValid()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isValid());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromRoot("").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromDrive('c', "").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromNetworkShare("server", "share", "").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory("").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromBase("", "").isValid());

					return {};
				}

				Ash::Test::Assertion isRelative()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isRelative());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromRoot("").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromDrive('c', "").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromNetworkShare("server", "share", "").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromCurrentDirectory("").isRelative());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromBase("", "").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromBase("c:", "").isRelative());

					return {};
				}

				Ash::Test::Assertion isAbsolute()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path("").isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromRoot("").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromDrive('c', "").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromNetworkShare("server", "share", "").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory("").isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromBase("", "").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromBase("c:", "").isAbsolute());

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
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path().reduce(),                                    L""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".").reduce(),                                 L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./..").reduce(),                              L".\\.."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./test/..").reduce(),                         L"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./test/../..").reduce(),                      L".\\.."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./../test/../../path").reduce(),              L".\\..\\..\\path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("c:").reduce(),                                L"c:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("c:/test").reduce(),                           L"c:\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("c:/..").reduce(),                             L"c:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("c:/test/..").reduce(),                        L"c:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("c:/test/../..").reduce(),                     L"c:"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("c:/../test/../../path").reduce(),             L"c:\\path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share").reduce(),                    L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share/test").reduce(),               L"\\\\server\\share\\test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share/..").reduce(),                 L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share/test/..").reduce(),            L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share/test/../..").reduce(),         L"\\\\server\\share"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("//server/share/../test/../../path").reduce(), L"\\\\server\\share\\path"));

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

					TEST_IS_FALSE(Ash::FileSystem::Path().getLastComponent(component));

					for (size_t n = 0; n < sizeof(testCases) / sizeof(testCases[0]); n++)
					{
						TEST_IS_TRUE(Ash::FileSystem::Path(testCases[n].path).getLastComponent(component));
						TEST_IS_EQ(component.getLength(), strlen(testCases[n].lastComponent));
						TEST_IS_ZERO(memcmp(component.at(0), testCases[n].lastComponent, component.getLength()));
					}

					Ash::Wide::String<> wideComponent;
					TEST_IS_TRUE(Ash::FileSystem::Path("./test").getLastComponent(wideComponent));
					TEST_IS_EQ(wideComponent.getLength(), wcslen(L"test"));
					TEST_IS_ZERO(memcmp(wideComponent.at(0), L"test", wideComponent.getLength()));

					Ash::Utf8::String<> utf8Component;
					TEST_IS_TRUE(Ash::FileSystem::Path("./test").getLastComponent(utf8Component));
					TEST_IS_EQ(utf8Component.getLength(), strlen("test"));
					TEST_IS_ZERO(memcmp(utf8Component.at(0), "test", utf8Component.getLength()));

					Ash::Ascii::String<> asciiComponent;
					TEST_IS_TRUE(Ash::FileSystem::Path("./test").getLastComponent(asciiComponent));
					TEST_IS_EQ(asciiComponent.getLength(), strlen("test"));
					TEST_IS_ZERO(memcmp(asciiComponent.at(0), "test", asciiComponent.getLength()));

					return {};
				}
#elif defined __linux__
				static inline bool isEqual(const Ash::FileSystem::Path &left, const Ash::FileSystem::Path::Encoding::Code *right)
				{
					Ash::FileSystem::Path::View leftValue = left.getValue();
					return (leftValue.getLength() == strlen((const char *)right)) && (strncmp((const char *)leftValue.at(0), (const char *)right, leftValue.getLength()) == 0);
				}

				static Ash::Test::Assertion core()
				{
					TEST_IS_TRUE(Ash::FileSystem::Path::supportRoot);
					TEST_IS_FALSE(Ash::FileSystem::Path::supportDrive);
					TEST_IS_FALSE(Ash::FileSystem::Path::supportNetworkShare);

					return {};
				}

				static Ash::Test::Assertion relative()
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

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".", "invalid/"),               u8""));

					return {};
				}

				static Ash::Test::Assertion fromRoot()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromRoot(""), u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/"),          u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/test/path"), u8"/test/path"));

					return {};
				}

				static Ash::Test::Assertion fromDrive()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('A', ""),          u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromDrive('C', "test/path"), u8""));

					return {};
				}

				static Ash::Test::Assertion fromNetworkShare()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", "share", ""),     u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", u8"share", ""),   u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare("server", L"share", ""),    u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", "share", ""),   u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", u8"share", ""), u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(u8"server", L"share", ""),  u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", "share", ""),    u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", u8"share", ""),  u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromNetworkShare(L"server", L"share", ""),   u8""));

					return {};
				}

				Ash::Test::Assertion fromBase()
				{
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("test", "path"),                                  u8"./test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase(Ash::FileSystem::Path("test", "path"), "file"),   u8"./test/path/file"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("/test", "path"),                                 u8"/test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase(Ash::FileSystem::Path::fromRoot("test"), "path"), u8"/test/path"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("/", "."),                                        u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase(Ash::FileSystem::Path::fromRoot(""), ""),         u8"/"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromBase("test", "/path"),                                 u8""));

					return {};
				}

				Ash::Test::Assertion fromCurrentDirectory()
				{
					Ash::FileSystem::Path currentDirectory = Ash::FileSystem::Path::fromCurrentDirectory("");
					TEST_IS_NOT_NULL(currentDirectory.getValue().at(0));
					TEST_IS_NOT_ZERO(*currentDirectory.getValue().at(0));

					Ash::FileSystem::Path directory1 = Ash::FileSystem::Path::fromCurrentDirectory("test/path");
					Ash::FileSystem::Path directory2 = Ash::FileSystem::Path::fromBase(currentDirectory, "test/path");
					TEST_IS_TRUE(isEqual(directory1, directory2));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path::fromCurrentDirectory("/test/path"), u8""));

					return {};
				}

				Ash::Test::Assertion isValid()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromRoot("").isValid());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromDrive('c', "").isValid());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromNetworkShare("server", "share", "").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory("").isValid());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromBase("", "").isValid());

					return {};
				}

				Ash::Test::Assertion isRelative()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isRelative());
					TEST_IS_TRUE(Ash::FileSystem::Path("").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromRoot("").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromDrive('c', "").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromNetworkShare("server", "share", "").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromCurrentDirectory("").isRelative());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromBase("", "").isRelative());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromBase("/", "").isRelative());

					return {};
				}

				Ash::Test::Assertion isAbsolute()
				{
					TEST_IS_FALSE(Ash::FileSystem::Path().isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path("").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromRoot("").isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromDrive('c', "").isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromNetworkShare("server", "share", "").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromCurrentDirectory("").isAbsolute());
					TEST_IS_FALSE(Ash::FileSystem::Path::fromBase("", "").isAbsolute());
					TEST_IS_TRUE(Ash::FileSystem::Path::fromBase("/", "").isAbsolute());

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
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path().reduce(),                       u8""));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path(".").reduce(),                    u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./..").reduce(),                 u8"./.."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./test/..").reduce(),            u8"."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./test/../..").reduce(),         u8"./.."));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("./../test/../../path").reduce(), u8"./../../path"));

					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/").reduce(),                    u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/test").reduce(),                u8"/test"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/..").reduce(),                  u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/test/..").reduce(),             u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/test/../..").reduce(),          u8"/"));
					TEST_IS_TRUE(isEqual(Ash::FileSystem::Path("/../test/../../path").reduce(),  u8"/path"));

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

					TEST_IS_FALSE(Ash::FileSystem::Path().getLastComponent(component));

					for (size_t n = 0; n < sizeof(testCases) / sizeof(testCases[0]); n++)
					{
						TEST_IS_TRUE(Ash::FileSystem::Path(testCases[n].path).getLastComponent(component));
						TEST_IS_EQ(component.getLength(), strlen(testCases[n].lastComponent));
						TEST_IS_ZERO(memcmp(component.at(0), testCases[n].lastComponent, component.getLength()));
					}

					Ash::Wide::String<> wideComponent;
					TEST_IS_TRUE(Ash::FileSystem::Path("./test").getLastComponent(wideComponent));
					TEST_IS_EQ(wideComponent.getLength(), wcslen(L"test"));
					TEST_IS_ZERO(memcmp(wideComponent.at(0), L"test", wideComponent.getLength()));

					Ash::Utf8::String<> utf8Component;
					TEST_IS_TRUE(Ash::FileSystem::Path("./test").getLastComponent(utf8Component));
					TEST_IS_EQ(utf8Component.getLength(), strlen("test"));
					TEST_IS_ZERO(memcmp(utf8Component.at(0), "test", utf8Component.getLength()));

					Ash::Ascii::String<> asciiComponent;
					TEST_IS_TRUE(Ash::FileSystem::Path("./test").getLastComponent(asciiComponent));
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
			TEST_CASE(Ash::Test::FileSystem::Path::relative),
			TEST_CASE(Ash::Test::FileSystem::Path::fromRoot),
			TEST_CASE(Ash::Test::FileSystem::Path::fromDrive),
			TEST_CASE(Ash::Test::FileSystem::Path::fromNetworkShare),
			TEST_CASE(Ash::Test::FileSystem::Path::fromBase),
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
