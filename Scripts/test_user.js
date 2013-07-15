
String.prototype.log = function() {
    var args = arguments;
    WScript.StdOut.WriteLine( this.replace(/{(\d+)}/g, function(match, number) {
        return typeof args[number] != 'undefined' ? args[number] : match;
        }) );
}


var advisor = new ActiveXObject('Ness.ScsInstallationAdvisor');

result = advisor.testUser( "london.home", "uri", "xxx" );
'test user result: {0}'.log( result );



