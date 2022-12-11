#include "Redactor_MVC.h"

int main()
{
    RedactorModel rm;
    RedactorController rc(&rm);
    std::shared_ptr<RedactorView> rv = RedactorView::create(&rm);

    rc.createDocument("doc1.xml");
    rc.importDocumentFromFile("doc2.xml");
    rc.exportDocumentToFile("doc2.xml", "doc3.xml");

    auto box = std::make_shared<Box>(Point{ 0,0 }, Point{ 10,10 });
    rc.createPrimitive("doc1.xml", box);
    rc.removePrimitive("doc2.xml", box);

    rc.importDocumentFromFile("doc3.xml");
    auto circle = std::make_shared<Circle>(Point{ 15,20 }, 5);
    rc.createPrimitive("doc3.xml", circle);
}