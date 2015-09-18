<?php
/**
 * @file
 * A tips and advice module that displays user information.
 */
function faculty_updateaccount($form, &$form_state){
    global $user;
    global $base_url;
    $query = db_select('faculty')
        ->fields('faculty')    
        ->condition('uid', $user->uid, '=')
        ->execute();  
    
    $count = $query->rowCount(); 
    if($count==0){
        drupal_goto("faculty/account");
    }
     
    $result = $query->fetchAll();
    $form = array();$i=0;
    foreach($result as $row){
        $photo= $row->photo;
        if($photo!='')
            $image = ' <img src="'.file_create_url($photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/profile_pic.jpg').'"/>';

    $form['aboutme' . $i]['info'] = array(
            '#type' => 'item', 
            '#markup' => '
                <h1>Welcome, ' . $row->name . '!!</h1>
                
            <div class="myaccount"> 
            <div class="myaccount-tit">
            <div class="createbutton"><a href="' . $base_url . '/'.get_admin_link().'faculty/editaccount">Edit Profile</a></div>
                </div>                                                 
                <div class="myaccount-cont">
                <div id="Education1" class="row" >  
                    <div class="col-md-2 upload_photo">
                         '.$image.'
                    </div>
                    <div class="col-md-10 myacount_content">
                        <ul class="employer-list">
                        <li><label>Application ID:</label> <div class="employer-cont">' . $user->uid. '</div></li>
                           <li><label>Name:</label> <div class="employer-cont">' . $row->name. '</div></li>
                            <li><label>LDAP Value:</label> <div class="employer-cont">' . $row->ldap_value. '</div></li>
                            <li><label>LDAP Type:</label> <div class="employer-cont">' . $row->ldap_type. '</div></li>                             
                            <li><label>Email: </label><span>'.$row->email.'</span></li>
                            <li><label>Date of Birth: </label><span>'.dateformat($row->dob).'</span></li>
                            <li><label>Mobile: </label><span>'.$row->mobile.'</span></li>
                            <li><label>Telephone: </label><span>'.$row->telephone.'</span></li>
                            <li><label>Nationality: </label><span>'.get_country_name($row->nationality).'</span></li>
                            <li><label>Location: </label><span>'.get_country_name($row->country).' '.get_country_name($row->state).'</span></li>
                            <li><label>Additional Info: </label><span>'.$row->additional_info.'</span></li>
                            <li><label>Department: </label><span>'.$row->department.'</span></li>
                            <li><label>Physical Department: </label><span>'.$row->physical_department.'</span></li>                            
                        </ul>                
                    </div>
                       
                </div>
                </div>                            
            </div>
                ',
                );
        $i++;         
    }
    
    return $form;
}