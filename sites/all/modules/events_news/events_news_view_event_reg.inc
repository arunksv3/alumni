<?php
function events_news_view_event_reg($id){
    global $base_url;
    global $user;
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $states_list = alumni_std_states_list();
    $id=(int)decrypt($id,$decrypting_key);
    $query = db_select('event_registrations', 'r');
    $query->join('events','e','e.id=r.event_id');
    $query->fields('r',array());
    $query->fields('e',array('event_code','title'))
    ->condition('r.id', $id, '=');
    $result = $query->execute()->fetchAll();   
    
    
    $form = array(); 
    foreach($result as $row){
         $output = '<div id="Event_Register_View">';
 
       $output.= '<h2>Event Registration</h2><div class="Event_Code"><label>Registration ID:</label><span> '.sprintf("ER-%04d", $row->id).'</span></div> ';
   
   
    $membershipArr = membership_type();
    //echo "<pre>";print_r($result);
  
       
        if($row->status==0)
            $status = 'Pending';
        else if($row->status==1)
            $status = 'Approved';
        else 
            $status = 'Rejected';
        
//         <li><label>Event Code:</label><span> '.$row->event_code.'</span></li>
//        <li><label>Designation: </label><span>'.$row->designation.'</span></li>         
//            <li><label>Membership Type:</label><span> '.$membershipArr[$row->membership_type].'</span></li>
//            <li><label>Company Name: </label><span>'.$row->company_name.'</span></li>
//            <li><label>Company Address: </label><span>'.$row->company_address.'</span></li> 
        $output .='<ul class="view-list">
           
            <li><label>Event Title:</label><span> '.$row->title.'</span></li>
            <li><label>Name:</label><span> '.$row->name.'</span></li>
            <li><label>I/C No: </label><span>'.$row->icno.'</span></li>
            <li><label>Email:</label><span> '.$row->email.'</span></li>
            <li><label>Mobile:</label><span> '.$row->mobile.'</span></li>
            <li><label>Telephone:</label><span> '.$row->telephone.'</span></li> 
            <li><label>Gender: </label><span>'.$row->gender.'</span></li>
            <li><label>Nationality:</label><span> '. $row->nationality.'</span></li>
            <li><label>Status:</label><span> '.$status.'</span></li>
            <li><label>Created Date: </label><span>'.dateformat($row->created_date).'</span></li>'
                . '</ul>'; 
        
         $output .= '</div>';
    }
    
    
    
    $form['list'] = array(
        '#type' => 'item',
        '#markup' =>$output,  
     ); 
    
    return $form;
}
