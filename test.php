<?php 
error_reporting(E_ALL);
// This code uses the START_TLS command

$adServer = "10.1.2.74"; //"ibnuhajar.iiu.edu.my"; //
$username  = "cn=acsd,cn=Users,dc=iiu,dc=edu,dc=my";
$password = "acsd#123";

echo "<h3>LDAP query test</h3>";

//echo "Connecting ...";

$ds = ldap_connect($adServer);  // must be a valid LDAP server!

//echo "connect result is " . $ds . "<br />";

ldap_set_option($ds, LDAP_OPT_PROTOCOL_VERSION, 3);

if ($ds) { 
    //echo "Binding ..."; 
    $r = ldap_bind($ds, $username, $password);     // this is an "anonymous" bind, typically    // read-only access

    //echo "Bind result is " . $r . "<br />";

    //echo "Searching for (sn=S*) ...";
//$arr = array('uid','sn');
$filter = "(cn=*)";
    // Search surname entry
	//$sr = ldap_search($ds, "cn=1020003,cn=Users,dc=iiu,dc=edu,dc=my", "cn=1020003"); 
    $sr = ldap_search($ds, $username, $filter);  
/*    echo "Search result is " . $sr . "<br />";

    echo "Number of entries returned is " . ldap_count_entries($ds, $sr) . "<br />";

    echo "Getting entries ...<p>";
    $info = ldap_get_entries($ds, $sr);
	
    echo "Data for " . $info["count"] . " items returned:<p>";
	echo '<pre>'; //print_r($info);

    /*for ($i=0; $i<$info["count"]; $i++) {
        print_r($info[$i]);
    }*/
$conn = oci_connect('acsd_user', '4csD#usr', '//10.1.2.224:1521/devitd.iium.edu.my' );

if (!$conn) {
    $e = oci_error();
    trigger_error(htmlentities($e['message'], ENT_QUOTES), E_USER_ERROR);
}

$stid = oci_parse($conn, "SELECT COUNT(*)  FROM UIA.CP_STATE_VW");
oci_execute($stid);

echo '<pre>';
while ($row = oci_fetch_array($stid, OCI_ASSOC+OCI_RETURN_NULLS)) {
	print_r($row);
   
}


  //  echo "Closing connection";
    ldap_close($ds);

} else {
    echo "<h4>Unable to connect to LDAP server</h4>";
}

