using System;
using System.Linq;
using System.Linq.Expressions;
using System.Web.Mvc;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;

using Diablo.DataAccessLayer.Data;
using Diablo.DataAccessLayer.Models;
using Diablo.MvcWebApp.Controllers;

namespace Diablo.MvcWebApp.Tests.Controllers
{
    [TestClass]
    public class DepartmentControllerTest
    {
        Mocks.MockDepartmentRepository objMockRepository;
        DepartmentController objController;
        
        [TestInitialize]
        public void TestInitialize()
        {
            
        }

        [TestCleanup]
        public void TestCleanUp()
        {
            objMockRepository = null;
            objController = null; 
        }                
        
        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Index_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Index();

            //assert
            Assert.IsTrue(objMockRepository.IsListCalled);
            Assert.IsInstanceOfType(((ViewResult)result).Model, typeof(IList<Department>));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Details_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Details(1);
                 
            //assert
            Assert.IsTrue(objMockRepository.IsFindCalled);
            Assert.IsInstanceOfType(((ViewResult)result).Model, typeof(Department));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Details_Missing_ID()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Details(0);
            var view = (ViewResult)result;

            //assert
            Assert.AreEqual(view.ViewBag.Message, "Missing id value", "Id should not be 0");
            Assert.IsFalse(objMockRepository.IsFindCalled);
          
            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Details_ID_HttpNotFound()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Details(99);

            //assert
            Assert.IsTrue(objMockRepository.IsFindCalled);
            Assert.IsInstanceOfType(result, typeof(System.Web.Mvc.HttpNotFoundResult));
            Assert.IsNotInstanceOfType(result, typeof(ViewResult));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Create_Get_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Create();

            //assert
            Assert.IsInstanceOfType(result, typeof(ViewResult));

            result = null;
        }
              
        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Create_Post_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var department = new Department { DepartmentId = 7, Name = "CleanUp", Description = "This place is a mess", Address = "4445 East Lansing Road", City = "Salt Lake City", State = "UT", PostalCode = "29854", IsActive = true, IsDeleted = false };
            var result = objController.Create(department);

            //assert
            Assert.IsTrue(objMockRepository.IsStoreCalled);
            Assert.IsInstanceOfType(result, typeof(RedirectToRouteResult));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Create_Post_NullDepartment()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            Department department = null;

            //act            
            var result = objController.Create(department);

            //assert
            Assert.IsFalse(objMockRepository.IsStoreCalled);
            Assert.IsInstanceOfType(result, typeof(System.Web.Mvc.HttpNotFoundResult));
        
            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Create_Post_ModelStateInvalid()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            var department = new Department();

            // you add this value on ModelState to force the error
            objController.ModelState.AddModelError("", "mock error message");

            //act            
            var result = objController.Create(department);

            //assert
            Assert.IsFalse(objMockRepository.IsStoreCalled);
            Assert.IsInstanceOfType(result, typeof(ViewResult));
            Assert.IsInstanceOfType(((ViewResult)result).Model, typeof(Department));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Delete_Get_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Delete(1);

            //assert
            Assert.IsInstanceOfType(((ViewResult)result).Model, typeof(Department));
            Assert.IsTrue(objMockRepository.IsFindCalled);

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Delete_Get_Missing_ID()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Delete(0);

            //assert
            Assert.IsInstanceOfType(result, typeof(System.Web.Mvc.HttpNotFoundResult));
            Assert.IsFalse(objMockRepository.IsFindCalled);

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartementController_Delete_Get_ID_Not_Found()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Delete(99);

            //assert
            Assert.IsInstanceOfType(result, typeof(System.Web.Mvc.HttpNotFoundResult));
            Assert.IsTrue(objMockRepository.IsFindCalled);
           
            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Delete_Post_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            
            //act
            var result = objController.DeleteConfirmed(1);

            //assert
            Assert.IsTrue(objMockRepository.IsDeleteCalled);
            Assert.IsInstanceOfType(result, typeof(RedirectToRouteResult));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Edit_Get_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);

            //act
            var result = objController.Edit(1);

            //assert
            Assert.IsTrue(objMockRepository.IsFindCalled);
            Assert.IsInstanceOfType(((ViewResult)result).Model, typeof(Department));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Edit_Get_Missing_ID()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            
            //act
            var result = objController.Edit(0);

            //assert
            Assert.IsFalse(objMockRepository.IsFindCalled);
            Assert.IsInstanceOfType(result, typeof(System.Web.Mvc.HttpNotFoundResult));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Edit_Get_ID_Not_Found()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            
            //act
            var result = objController.Edit(99);

            //assert
            Assert.IsTrue(objMockRepository.IsFindCalled);
            Assert.IsInstanceOfType(result, typeof(System.Web.Mvc.HttpNotFoundResult));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Edit_Post_Successfull()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            
            var department = new Department { DepartmentId = 7, Name = "CleanUp", Description = "This place is a mess", Address = "4445 East Lansing Road", City = "Salt Lake City", State = "UT", PostalCode = "29854", IsActive = true, IsDeleted = false };
           
            //act
            var result = objController.Edit(department);

            //assert
            Assert.IsTrue(objMockRepository.IsStoreCalled);
            Assert.IsInstanceOfType(result, typeof(RedirectToRouteResult));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Edit_Post_ModelStateInvalid()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            objController.ModelState.AddModelError("", "mock error message");
            var department = new Department();

            //act
            var result = objController.Edit(department);

            //assert
            Assert.IsFalse(objMockRepository.IsStoreCalled);
            Assert.IsInstanceOfType(result, typeof(ViewResult));
            Assert.IsInstanceOfType(((ViewResult)result).Model, typeof(Department));

            result = null;
        }

        [TestMethod]
        [TestCategory("Unit Tests")]
        [TestProperty("Controller", "Department")]
        public void DepartmentController_Edit_Post_NullDepartment()
        {
            //arrange
            objMockRepository = new Mocks.MockDepartmentRepository();
            objController = new DepartmentController(objMockRepository);
            Department department = null;

            //act
            var result = objController.Edit(department);

            //assert
            Assert.IsFalse(objMockRepository.IsStoreCalled);
            Assert.IsInstanceOfType(result, typeof(System.Web.Mvc.HttpNotFoundResult));

            result = null;
        }       
               
        private IList<Department> MockDepartmentDataSet()
        {
            var items = new System.Collections.Generic.List<Department>
            {
                new Department { DepartmentId = 1, Name = "Accounting", Description = "A place for numbers", Address = "4101 Hearthside Drive", City = "Traverse City", State = "MI", PostalCode = "43196", IsActive = true, IsDeleted = false },
                new Department { DepartmentId = 2, Name = "Development", Description = "This is how code gets done", Address = "123 West Ave Suite 200", City = "Columbus", State = "OH", PostalCode = "99183", IsActive = true, IsDeleted = false },
                new Department { DepartmentId = 3, Name = "Operations", Description = "I love the game Operation", Address = "4451 Rodoe Drive", City = "Denver", State = "CO", PostalCode = "80123", IsActive = true, IsDeleted = false },
                new Department { DepartmentId = 4, Name = "Marketing", Description = "Pretty pictures", Address = "4445 East Lansing Road", City = "Salt Lake City", State = "UT", PostalCode = "29854", IsActive = true, IsDeleted = false }
            };

            return items;
        }    
    }
}
