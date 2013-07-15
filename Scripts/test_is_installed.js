
String.prototype.log = function() {
    var args = arguments;
    WScript.StdOut.WriteLine( this.replace(/{(\d+)}/g, function(match, number) {
        return typeof args[number] != 'undefined' ? args[number] : match;
        }) );
}


var advisor = new ActiveXObject('Ness.ScsInstallationAdvisor');
isInstalled = advisor.isScsInstalled;
'Scs installed: {0}'.log( isInstalled ? 'yes' : 'no' );

