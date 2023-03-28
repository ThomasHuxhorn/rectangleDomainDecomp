
#include <QtTest>

// add necessary includes here

class TestRectangleDomainDecomp : public QObject
{
    Q_OBJECT

public:
    TestRectangleDomainDecomp();
    ~TestRectangleDomainDecomp();

private slots:
    void test_case1();

};

TestRectangleDomainDecomp::TestRectangleDomainDecomp()
{

}

TestRectangleDomainDecomp::~TestRectangleDomainDecomp()
{

}

void TestRectangleDomainDecomp::test_case1()
{

}

QTEST_APPLESS_MAIN(TestRectangleDomainDecomp)

#include "tst_testrectangledomaindecomp.moc"
