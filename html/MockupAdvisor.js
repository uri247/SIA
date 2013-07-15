//
// MockupAdvisor is a mockup implementation to enable development of the UI while the real ActiveX 
// Advisor is not installed, or when the host isn't Internet Explorer.
// Testers also use a similar object to test the business logic.
//

function MockupAdvisor() {

    this.computerName = "SampleComputerName";
    this.currentUser = "SAMPLE_DOMAIN\\sample_user";
    this.findSqlServers = function () { return ["dbsrv1", "dbsrv2"]; };
    this.findSchemasInServer = function (srv) { return [srv.concat('A'), srv.concat('B')]; };

    this.installAll = function () {
        // the mock up do nothing, but nevertheless fire event after 10 seconds. We are assuming here we are
        // running in a browser. If this is a problem replace with something else.
        window.setTimeout(
            (function (_this) {
                return function () { _this.oncomplete(0); };
            })(this), 2000);
    }

    this.installParams = {};

    this.testUser = function (domain, user, passwd) {
        return (domain !== '' && user !== '') ? "OK" : "FAIL";
    }
}

