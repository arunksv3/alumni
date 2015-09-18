<?php
/**
 * @file
 * A tips and advice module that displays user information.
 */
function tips_advice_download($filename){ 
    $key = variable_get('IIUM_Encryption_Key', NULL);
    $filename = decrypt($filename, $key);           
    ignore_user_abort(true);
    set_time_limit(0); // disable the time limit for this script

    $path = drupal_realpath(variable_get('IIUM_tips_advice_attachment_path', NULL));
    $dl_file = preg_replace("([^\w\s\d\-_~,;:\[\]\(\].]|[\.]{2,})", '', $filename); // simple file name validation
    $dl_file = filter_var($dl_file, FILTER_SANITIZE_URL); // Remove (more) invalid characters
    $fullPath = $path.'/'.$dl_file;

    if ($fd = fopen($fullPath, "r")) {
        $fsize = filesize($fullPath);
        $path_parts = pathinfo($fullPath);
        $ext = strtolower($path_parts["extension"]);
        switch ($ext) {
            case "pdf":
            header("Content-type: application/pdf");
            header("Content-Disposition: attachment; filename=\"".$path_parts["basename"]."\""); // use 'attachment' to force a file download
            break;
            // add more headers for other content types here
            default;
            header("Content-type: application/octet-stream");
            header("Content-Disposition: filename=\"".$path_parts["basename"]."\"");
            break;
        }
        header("Content-length: $fsize");
        header("Cache-control: private"); //use this to open files directly
        while(!feof($fd)) {
            $buffer = fread($fd, 2048);
            echo $buffer;
        }
    }
    fclose ($fd);
    exit;
}