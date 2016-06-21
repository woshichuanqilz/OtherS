using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Diablo.DataAccessLayer.Data;
using Diablo.DataAccessLayer.Models;
using Diablo.MvcWebApp.Controllers;

namespace Diablo.MvcWebApp.Tests.Mocks
{
    /// <summary>
    /// Mock Company Notes Repository
    /// </summary>
    class MockDepartmentRepository : IDepartmentRepository
    {
        public bool IsStoreCalled { get; set; }
        public bool IsSaveChangesCalled { get; set; }
        public bool IsDeleteCalled { get; set; }
        public bool IsFindCalled { get; set; }
        public bool IsListCalled { get; set; }

        public IList<Department> List
        {
            get
            {
                IsListCalled = true;
                return new System.Collections.Generic.List<Department>();
            }
        }

        public IQueryable<Department> QueryList
        {
            get { throw new NotImplementedException(); }
        }

        public void Store(Department department)
        {
            this.IsStoreCalled = true;
        }

        public Department Find(int id)
        {
            this.IsFindCalled = true;

            if (id == 99)
            {
                return null;
            }
            else
            {
                return new Department();
            }
        }

        public int SaveChanges()
        {
            this.IsSaveChangesCalled = true;
            return 1;
        }

        public void Delete(int id)
        {
            this.IsDeleteCalled = true;
        }
    }
}
