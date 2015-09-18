<?php
function events_news_view_event($id){
    global $base_url;
    global $user;
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $id=(int)decrypt($id,$decrypting_key);
    $event = db_select('events')
        ->fields('events')
        ->condition('id',$id,'=')
         ->execute();
    
    $event_attachments = db_select('event_attachments')
        ->fields('event_attachments')
        ->condition('event_id',$id,'=')
         ->execute();
    
    
    $form = array();
    
    $form['event'] = array(
        '#type' => 'item', 
        '#title' => ''
    );
    $output = '<div class="block block-system clearfix view-news-events">';
   
    $organizedArr = organized(); 
    foreach($event as $row) { 
        if($row->photo!='')
            $image = ' <img src="'.file_create_url($row->photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $output .='<div class="EN-details-header">
            <span class="date-time">'.dateformat($row->from_date).' To '.dateformat($row->to_date).' '.$row->from_time.' '.$row->to_time.'</span>
            <h1>'.$row->title.'</h1>          
            <div class="EN-header-right">
               <label>Event ID :</label>
               <span>'.sprintf("EV-%04d", $row->id).'</span>
            </div>
        </div>
        <div class="EN-details-content">
        <ul class="en-details-list">
             
           <li>
                <label>Organized By:</label>
                <span>'.$organizedArr[$row->organized_by].'</span> 
            </li>
            <li>
                <label>Venue:</label> 
                <span>'.$row->location.' '.get_country_name($row->country).' '.get_state_name($row->state).'</span>
            </li>
        </ul>
        <div class="EN-content-img">
        '.$image.'
        </div>
        <p>'.$row->description.'</p> 
        <ul class="en-details-list"><li>Attachment:';
        foreach($event_attachments as $res) {
            $output .='
                <a href="'.file_create_url($res->attachment).'" target="_blank">download</a>
             </li>';
        }
        $output .='</ul>';
       
    }
    
    $output .= '</div>';
    
    $form['events']['list'] = array(
        '#type' => 'item',         
        '#markup' => $output,         
     ); 
    
    return $form;
}