
String.prototype.log = function() {
    var args = arguments;
    WScript.StdOut.WriteLine( this.replace(/{(\d+)}/g, function(match, number) {
        return typeof args[number] != 'undefined' ? args[number] : match;
        }) );
}


var advisor = new ActiveXObject('Ness.ScsInstallationAdvisor');

dhcp_test = advisor.checkDhcpOption15();
'dhcp result: {0}'.log( dhcp_test );



