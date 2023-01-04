#pragma once
#include "Document.h"
#include <cassert>
#include <list>

class Observer 
{
public:
    virtual ~Observer() = default;

    virtual void update() = 0;
};

class Observable 
{
public:
    virtual ~Observable() = default;

    virtual void subscribe(const std::shared_ptr<Observer>& obs) = 0;
};


class RedactorModel : public Observable
{
public:
    void subscribe(const std::shared_ptr<Observer>& obs) override
    {
        m_subs.emplace_back(obs);
    }

    void createDocument(const std::string& name)
    {
        Document d(name, DocumentState::created);
        documents.emplace_back(std::move(d));
        activeDocIt = end(documents) - 1;
        notify();
    }

    void importDocumentFromFile(const std::string& fileName)
    {
        Document d(fileName, DocumentState::imported);
        documents.emplace_back(std::move(d));
        activeDocIt = end(documents) - 1;
        notify();
    }

    void exportDocumentToFile(const std::string& docName, const std::string& fileName)
    {
        auto doc = std::find(begin(documents), end(documents), docName);
        doc->export_to_file(fileName);
        activeDocIt = doc;
        notify();
    }

    void createPrimitive(const std::string& docName, std::shared_ptr<IDrawPrimitive> primitive)
    {
        auto doc = std::find(begin(documents), end(documents), docName);
        doc->create_primitive(primitive);
        activeDocIt = doc;
        notify();
    }

    void removePrimitive(const std::string& docName, std::shared_ptr<IDrawPrimitive> primitive)
    {
        auto doc = std::find(begin(documents), end(documents), docName);
        if (doc != end(documents))
        {
            if (doc->remove_primitive(primitive))
            {
                activeDocIt = doc;
                notify();
            }
        }
    }

    auto get_active_document() const
    {
        return *activeDocIt;
    }

    void notify()
    {
        auto iter = m_subs.begin();
        while (iter != m_subs.end())
        {
            auto ptr = iter->lock();
            if (ptr)
            {  // Если наблюдатель ещё существует, то оповещаем его
                ptr->update();
                ++iter;
            }
            else
            {  // Иначе - удаляем из списка наблюдателей.
                m_subs.erase(iter++);
            }
        }
    }
private:
    std::list<std::weak_ptr<Observer>> m_subs;
    std::vector<Document> documents;
    std::vector<Document>::iterator activeDocIt;
};


class RedactorView : public Observer, public std::enable_shared_from_this<RedactorView>
{
public:
    // Объект должен быть создан в динамической памяти.
    static std::shared_ptr<RedactorView> create(RedactorModel* model)
    {
        auto ptr = std::shared_ptr<RedactorView>{ new RedactorView{} };
        ptr->setRedactorModel(model);
        return ptr;
    }

    void update() override
    {
        assert(m_redactorModel);

        auto doc = m_redactorModel->get_active_document();
        auto docState = doc.get_state();
        switch (docState)
        {
        case DocumentState::created:
            std::cout << "document " << doc.get_name() << " created" << std::endl;
            break;
        case DocumentState::imported:
            std::cout << "document " << doc.get_name() << " imported" << " from file "
                << doc.get_name() << std::endl;
            break;
        case DocumentState::exported:
            std::cout << "document " << doc.get_name() << " exported" << " to file "
                << doc.get_exported_name() << std::endl;
            break;
        case DocumentState::primitiveCreated:
            std::cout << "in document " << doc.get_name() << " primitive created: " << std::endl;
            doc.get_active_primitive()->draw();
            break;
        case DocumentState::primitiveRemoved:
            std::cout << "in document " << doc.get_name() << " primitive removed" << std::endl;
            break;
        default:
            break;
        }
    }
private:
    // Чтобы пользователи не создавали объект в стеке, сделаем конструктор закрытым.
    RedactorView()
    {
    }

    void setRedactorModel(RedactorModel* model)
    {
        m_redactorModel = model;
        m_redactorModel->subscribe(shared_from_this());
    }

    RedactorModel* m_redactorModel = nullptr;
};

class RedactorController
{
public:
    RedactorController(RedactorModel* _model)
        : model(_model)
    {

    }

    void createDocument(const std::string& name)
    {
        model->createDocument(name);
    }

    void importDocumentFromFile(const std::string& fileName)
    {
        model->importDocumentFromFile(fileName);
    }

    void exportDocumentToFile(const std::string& docName, const std::string& fileName)
    {
        model->exportDocumentToFile(docName, fileName);
    }

    void createPrimitive(const std::string& docName, std::shared_ptr<IDrawPrimitive> primitive)
    {
        model->createPrimitive(docName, primitive);
    }

    void removePrimitive(const std::string& docName, std::shared_ptr<IDrawPrimitive> primitive)
    {
        model->removePrimitive(docName, primitive);
    }

private:
    RedactorModel* model;
};
