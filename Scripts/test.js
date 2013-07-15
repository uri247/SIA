
String.prototype.log = function() {
    var args = arguments;
    WScript.StdOut.WriteLine( this.replace(/{(\d+)}/g, function(match, number) {
        return typeof args[number] != 'undefined' ? args[number] : match;
        }) );
}


var advisor = new ActiveXObject('Ness.ScsInstallationAdvisor');
'type of advisor: {0}'.log( typeof(advisor) );

isInstalled = advisor.isScsInstalled;
'Scs installed: {0}'.log( isInstalled ? 'yes' : 'no' );

params = advisor.installParams;
'default install parameters:'.log();
'create database: {0}'.log( params.createDatabase ? 'yes' : 'no' );
'install service: {0}'.log( params.installService ? 'yes' : 'no' );
'install console: {0}'.log( params.installConsole ? 'yes' : 'no' );

params.createDatabase = true;
params.installService = true;
params.installConsole = false;
params.serviceAccountDomain = 'LONDON';
params.serviceAccountUser = 'uri';
params.serviceAccountPassword = '';
//params.installerLocation = 'c:\\IntelSCS\\Resources\\IntelSCS\\RCS\\IntelSCSInstaller.exe';
params.installerLocation = 'c:\\windows\\system32\\notepad.exe';
'create database: {0}'.log( params.createDatabase ? 'yes' : 'no' );
'install service: {0}'.log( params.installService ? 'yes' : 'no' );
'install console: {0}'.log( params.installConsole ? 'yes' : 'no' );
'service account: {0}\\{1} ({2})'.log( params.serviceAccountDomain, params.serviceAccountUser, params.serviceAccountPassword );

advisor.oncomplete = function () {
    'hey'.log();
};

advisor.installAll();

WScript.StdIn.ReadLine();

