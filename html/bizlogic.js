/*
 * Object:      viewModel
 *
 * Description: This is almost a classic "view model" in a MVVM design pattern. It is short of a full
 *              mvvm modle by not sending property change (a la WPF or knockout.js), but in the sense
 *              of representing the state of the view as well as by being view-agnostics (note there is
 *              no html information in this script). Theoritecally, we could implement a different "view"
 *              for this ViewModel for testing.
 *
 *              The view model is also responsible for communicating with the advisor object for diagnostics
 *              and execution.
 *
 * Author:      Uri London
 *
 */


function ViewModel() {
    this.oneclick = true;
    this.createDb = true;
    this.installSvc = true;
    this.insatllConsole = true;
    this.svcAccntDomain = '';
    this.svcAccntName = '';
    this.svcAccntPassword = '';
    this.sqlServer = '.\\SQLEXPRESS';
    this.sqlSchema = 'IntelSCS';
    this.acceptEula = true;
    //this.installerLocation = 'c:\\windows\\system32\\notepad.exe';
    //this.installerLocation = 'c:\\IntelSCS\\Resources\\IntelSCS\\RCS\\IntelSCSInstaller.exe';
    this.installerLocation = 'RCS\\IntelSCSInstaller.exe';
}


ViewModel.prototype.init = function() {
    this.advisor = this.getAdvisor();

    this.computerName = this.advisor.computerName;
    this.firePropChanged('computerName', this.computerName);

    this.currentUser = this.advisor.currentUser;
    this.firePropChanged('currentUser', this.currentUser);

    this.advisor.oncomplete = ( function(_this) {
        return function(status) {
            if (_this.oncomplete) {
                _this.oncomplete(status);
            }
        };
    } )(this);
}

ViewModel.prototype.getAdvisor = function () {
    var advisor = undefined;
    try {
        //throw 42;
        advisor = new ActiveXObject('Ness.ScsInstallationAdvisor');
    }
    catch (err) {
        advisor = new MockupAdvisor();
        this.firePropChanged('mockup', true);
    }


    return advisor;
};


ViewModel.prototype.installNow = function () {
    params = this.advisor.installParams;
    params.createDatabase = this.createDb;
    params.installService = this.installSvc;
    params.installConsole = this.installConsole;
    params.serviceAccountDomain = this.svcAccntDomain;
    params.serviceAccountUser = this.svcAccntName;
    params.serviceAccountPassword = this.svcAccntPassword;
    params.installerLocation = this.installerLocation;
    params.sqlServer = this.sqlServer;
    params.sqlSchema = this.sqlSchema;
    this.advisor.installAll();
};

ViewModel.prototype.findSqlServers = function () {
    return this.advisor.findSqlServers();
}

ViewModel.prototype.testServiceData = function () {
    if (!this.installSvc) {
        return true;
    }
    else {
        result = this.advisor.testUser(this.svcAccntDomain, this.svcAccntName, this.svcAccntPassword);
        return (result === 'OK');
    }
}


ViewModel.prototype.findSchemasInServer = function (srv)
{
    return this.advisor.findSchemasInServer(srv);
};


ViewModel.prototype.firePropChanged = function (prop, val) {
    if (this.onpropchanged) {
        this.onpropchanged(prop, val);
    }
}


/*
 * object:      pages
 *
 * Description: This object maintain the list of pages in the Advisor Wizard. Since navigation is dynamic (
 *              i.e. next page and previous page are sensitive to user input), the logic is encoded within
 *              the next/prev functions.
 */

var pages = {

    welcome: {
        selector: '#welcome',
        next: function () { return pages.eula; },
        prev: function () { return undefined; },
        has_next: function () { return true; },
        has_prev: function () { return false; }
    },

    eula: {
        selector: '#eula',
        next: function () { return pages.oneclick_or_custom; },
        prev: function () { return pages.welcome; },
        has_next: function () { return viewModel.acceptEula ? true : false; },
        has_prev: function () { return true; }
    },

    oneclick_or_custom: {
        selector: '#oneclick-or-custom',
        next: function () { return viewModel.oneclick ? pages.install : pages.install_service; },
        prev: function () { return pages.eula; },
        has_next: function () { return true; },
        has_prev: function () { return true; },
    },


    install_service: {
        selector: '#install-service',
        next: function () { return pages.create_db; },
        prev: function () { return pages.oneclick_or_custom; },
        has_next: function () { return true; },
        has_prev: function () { return true; },
        leave: function () { return viewModel.testServiceData(); }
    },

    create_db: {
        selector: '#create-db',
        next: function () { return pages.install_console; },
        prev: function () { return pages.install_service; },
        has_next: function () { return true; },
        has_prev: function () { return true; },
    },

    install_console: {
        selector: '#install-console',
        next: function () { return pages.install; },
        prev: function () { return pages.create_db; },
        has_next: function () { return true; },
        has_prev: function () { return true; },
    },

    install: {
        selector: '#install',
        next: function () { return undefined; },
        prev: function () { return viewModel.oneclick ? pages.oneclick_or_custom : pages.install_console; },
        has_next: function () { return false; },
        has_prev: function () { return true; }
    },

    report: {
        selector: '#report',
        next: function () { return undefined; },
        prev: function () { return undefined; },
        has_next: function () { return false; },
        has_prev: function () { return false; }
    },

};

