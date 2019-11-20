/*! \file */

/*

This file is part of ENiX4.

ENiX4 is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiX4 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiX4.  If not, see <http://www.gnu.org/licenses/>.

  __    _______   ___ _  __    __ ____  
 / /   / ____/ | / (_) |/ /   / // /\ \ 
/ /   / __/ /  |/ / /|   /   / // /_ \ \
\ \  / /___/ /|  / //   |   /__  __/ / /
 \_\/_____/_/ |_/_//_/|_|     /_/   /_/ 
                                        


Programmed By M.S.Braithwaite 2014.

*/

#include <string.h>
#include "ENiX_WMSAPI.h"
#include <map>
//#include "ENiX_Settings.h"

vector<string> WMSConversion::WMS2Vec(WMS_Rec Rec){
  char FormattedLine[1024];
  string StrLine;
  string ThickLine="============================================================";
  string ThinLine ="------------------------------------------------------------";
  vector<string> DisplayOutput;
  DisplayOutput.push_back(ThickLine);
  if((!Rec.Contents.size())||((Rec.Contents[0]).size()<8)){
    DisplayOutput.push_back("Cannot display record.");
    DisplayOutput.push_back(ThickLine);
    return {};
  }    
  snprintf(FormattedLine,1023,"Name %23s Grammar %23s",(Rec.Contents[0])[0].Convert2Str().c_str(),(Rec.Contents[0])[1].Convert2Str().c_str());
  DisplayOutput.push_back(FormattedLine);
  snprintf(FormattedLine,1023,"Purpose %20s Emotion %23s",(Rec.Contents[0])[2].Convert2Str().c_str(),(Rec.Contents[0])[3].Convert2Str().c_str());
  DisplayOutput.push_back(FormattedLine);
  snprintf(FormattedLine,1023,"First Time %28s",(Rec.Contents[0])[4].Convert2Str().c_str());
  DisplayOutput.push_back(FormattedLine);
  snprintf(FormattedLine,1023,"Last Time %29s",(Rec.Contents[0])[5].Convert2Str().c_str());
  DisplayOutput.push_back(FormattedLine);
  snprintf(FormattedLine,1023,"Probability %16s Commands %22s",(Rec.Contents[0])[6].Convert2Str().c_str(),(Rec.Contents[0])[7].Convert2Str().c_str());
  DisplayOutput.push_back(FormattedLine);
  if((Rec.Contents.size()>1)&&(Rec.Contents[1].size())){
    DisplayOutput.push_back(ThinLine);
    for(size_t AttrId=0;AttrId<(Rec.Contents[1]).size()&&AttrId<(Rec.Contents[2]).size();++AttrId){
      snprintf(FormattedLine,1023,"%-30s%30s",(Rec.Contents[1])[AttrId].Convert2Str().c_str(),(Rec.Contents[2])[AttrId].Convert2Str().c_str());
      DisplayOutput.push_back(FormattedLine);
    }
  }
  DisplayOutput.push_back(ThickLine);
  return DisplayOutput;
}

void WMSConversion::ProperPrintWMS(WMS_Rec Rec){
  vector<string> Data=WMS2Vec(Rec);
  for(size_t _=0;_<Data.size();++_)
    puts(Data[_].c_str());
}

bool WMSAPI::DeleteAmoeba(unsigned long int StaticId){
  Amoeba_Rec DeletedRecord;
  DeletedRecord=Bin2Amoeba(GetData(StaticId));
  if(DeletedRecord.RefSeg.size()){
    for(size_t _=0;_<(DeletedRecord.RefSeg[0]).size();_+=3){
      unsigned long int RefRecordId=(DeletedRecord.RefSeg[0])[_];
      size_t RefSegId=(DeletedRecord.RefSeg[0])[_+1];
      unsigned long int RefAttrId=(DeletedRecord.RefSeg[0])[_+2];
      Amoeba_Rec ModifyRec=Bin2Amoeba(GetData(RefRecordId));      
      if((RefSegId<4)&&(RefSegId>1)){
	ModifyRec.RefSeg[2]=BinDataSetDel(ModifyRec.RefSeg[2],RefAttrId);
	ModifyRec.RefSeg[3]=BinDataSetDel(ModifyRec.RefSeg[3],RefAttrId);
      }
      else
	ModifyRec.RefSeg[RefSegId]=BinDataSetDel(ModifyRec.RefSeg[RefSegId],RefAttrId);
      BinData BinaryRec=Amoeba2Bin(ModifyRec);
      RepData(BinaryRec,RefRecordId);
    }  
  }
  DelData(StaticId);  
}


bool WMSAPI::DelFromPP(BinData ToDelete){
  /* remove the the record from the PP */
  
  RecFilter=BinDataSetDel(RecFilter,ToDelete);
  unsigned long int Pos=Search(ActivePP);
  WMS_Rec ToUpdate=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));
  ToUpdate.Contents[1]=RecFilter;
  vector<Amoeba_Rec> AmoebaData=WMS2Amoeba(ToUpdate);
  WriteAll2DB(AmoebaData);  
  return true;
}

bool WMSAPI::DelFromPP(unsigned long int DelPos){
  /* remove the the record from the PP */
  BinData Name=GetName(DelPos);
  return DelFromPP(Name);
}

WMS_Rec WMSAPI::MergeFilteredWMSToDB(WMS_Rec Source){
  WMS_Rec FilteredOut={};
  BinData SourceName={};

  AddRec2PP(Source);

  if(Source.Contents.size()>2){
    if(Source.Contents[0].size())
      SourceName=(Source.Contents[0])[0];
    else
      return Source;
  }
  else{
    size_t Size=Source.Contents.size();
    vector<BinData> Segment={};
    Source.Contents.resize(3);
    if(Size==1) 
      Source.Contents[1]=Segment;
    Source.Contents[2]=Segment;
  }
  unsigned long int Pos=Search(SourceName);
  if(Pos==-1){
    WMS_Rec NewRec;
    NewRec.Contents.resize(3);
    NewRec.Contents[0]=Source.Contents[0];
    NewRec.Contents[1]=Source.Contents[1];
    NewRec.Contents[2]=Source.Contents[2];
    FilteredOut=NewRec;
  }
  else{
    FilteredOut=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));
  }
  WMS_Rec Merged=MergeFilters(Source,FilteredOut);
  return Merged;
}

WMS_Rec WMSAPI::InFilter(WMS_Rec Source){
  WMS_Rec Output={};  
  Output.Contents.resize(3);

  /* cache personality data */
  unsigned long int Pos=Search(ActivePP);
  WMS_Rec PPListRecord=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));

  vector<BinData> Filter=PPListRecord.Contents[2];

  /* copy the header */
  Output.Contents[0]=Source.Contents[0];
  
  /* Separate the Source record into a list of items.
     then compare that list with the Filter. */
  for(size_t _=0;(_<Source.Contents[1].size())&&(_<Source.Contents[2].size());_++){
    vector<BinData> ConvertedTokens=String2List((Source.Contents[2])[_],false);
    bool AllFound=true;
    for(size_t C=0;C<ConvertedTokens.size();C++){
      bool Found=false;
      for(size_t B=0;B<Filter.size();B++){
	if(Filter[B]==ConvertedTokens[C]){
	  Found=true;
	  break;
	}
      }      
      if(Found==false){
	AllFound=false;
      }
    }

    if(AllFound==true){
      // Add the attributes outside of the filter to the output record.
      Output.Contents[1].push_back((Source.Contents[1])[_]);
      Output.Contents[2].push_back((Source.Contents[2])[_]);
    }
    
  }
  return Output;
}

WMS_Rec WMSAPI::OutFilter(WMS_Rec Source){
  WMS_Rec Output={};  
  Output.Contents.resize(3);

  /* cache personality data */
  unsigned long int Pos=Search(ActivePP);
  WMS_Rec PPListRecord=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));
  
  vector<BinData> Filter=PPListRecord.Contents[2];

  /* copy the header */
  Output.Contents[0]=Source.Contents[0];
  
  /* Separate the Source record into a list of items.
     then compare that list with the Filter. */
  for(size_t _=0;(_<PPListRecord.Contents[1].size())&&(_<PPListRecord.Contents[2].size());_++){
    bool AllFound=true;
    vector<BinData> ConvertedTokens=String2List(Filter[_],false);
    for(size_t B=0;B<ConvertedTokens.size();B++){

      /* One of these must not be found. */
      bool Found=false;      
      for(size_t C=0;C<Filter.size();C++)
	if(Filter[C]==ConvertedTokens[B]){
	  Found=true;
	  break;
	}
      if(Found==false)
	AllFound=false;
    }    
    if(AllFound==false){

      /* Add the attributes outside of the filter to the output record. */
      Output.Contents[1].push_back((PPListRecord.Contents[1])[_]);
      Output.Contents[2].push_back((PPListRecord.Contents[2])[_]);
    }
  }
  return Output;
}

WMS_Rec WMSAPI::MergeFilters(WMS_Rec InRec,WMS_Rec OutRec){
  vector<BinData> InAttrList={},InValueList={},OutAttrList={},OutValueList={};
  vector<BinData> Data0={},Data1={},Data2={};
  
  WMS_Rec NewRec={};

  /* cache data input data */
  if(InRec.Contents.size()>2){
    InAttrList=InRec.Contents[1];
    InValueList=InRec.Contents[2];
  }
  if(OutRec.Contents.size()>2){
    OutAttrList=OutRec.Contents[1];
    OutValueList=OutRec.Contents[2];
  }
  
  Data0=InRec.Contents[0];
  Data1=InRec.Contents[1];
  Data2=InRec.Contents[2];
  
  
  /* For each Data we need to check to see if the 
     output record has the appropriate. */
  for(size_t C=0;(C<OutAttrList.size())&&(C<OutValueList.size());C++){
    bool Found=false;
    for(size_t _=0;(_<Data1.size())&&(_<Data2.size());_++)
      if((OutAttrList[C]==Data1[_])&&(OutValueList[C]==Data2[_]))
	Found=true;
    if(!Found){
      Data1.push_back(OutAttrList[C]);
      Data2.push_back(OutValueList[C]);
    }
  }
  
  NewRec.Contents.resize(3);
  NewRec.Contents[0]=Data0;
  NewRec.Contents[1]=Data1;
  NewRec.Contents[2]=Data2;
  
  return NewRec;
}

WMSAPI::WMSAPI(class SHMConfig DBConfig):Amoeba(DBConfig){
//WMSAPI::WMSAPI():Amoeba(666){  
  printf("Initialising Amoeba...            \r");
  fflush(stdout);
  BinData PersonalityProfile("unknown");
  SetPP(PersonalityProfile);
}

vector<unsigned long int> WMSAPI::GetAllPPRec(){
  unsigned long int Pos=Search(ActivePP);
  if(Pos==-1)
    return {};
  Amoeba_Rec PPAmoebaRec=Bin2Amoeba(GetData(Pos));
  return PPAmoebaRec.RefSeg[2];
}

void WMSAPI::DeletePP(BinData PPName){
  BinData BinRecName(":pprofiles");
  bool Removed=false;
  unsigned long int Pos=Search(BinRecName);
  WMS_Rec PPListRecord=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));
  if(PPListRecord.Contents.size()<3)
    return;
  for(size_t _=0;(_<PPListRecord.Contents[1].size())&&(_<PPListRecord.Contents[2].size());_++)
    if((PPListRecord.Contents[1])[_]==PPName){
      PPListRecord.Contents[1]=BinDataSetDel(PPListRecord.Contents[1],_);
      PPListRecord.Contents[2]=BinDataSetDel(PPListRecord.Contents[2],_);
      Removed=true;
      break;
    }
  if(Removed){
    if(PPName==ActivePP){
      BinData PersonalityProfile("unknown");
      SetPP(PersonalityProfile);      
    }
    vector<Amoeba_Rec> AmoebaData=WMS2Amoeba(PPListRecord);
    WriteAll2DB(AmoebaData);
  }
}

void WMSAPI::DeletePP(string PPName){
  BinData SearchToken(PPName);
  DeletePP(SearchToken);
}

void WMSAPI::PopulateFilter(BinData PPName){
  unsigned long int Pos=Search(ActivePP);
  WMS_Rec SourceRec=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));
  RecFilter=SourceRec.Contents[1];
  AttrFilter=SourceRec.Contents[2];
}

void WMSAPI::AddRec2PP(WMS_Rec Rec2Add){
  unsigned long int Pos=Search(ActivePP);
  WMS_Rec ToUpdate=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));

  // Add the record name to the PP
  RecFilter=BinDataSetAdd(RecFilter,(Rec2Add.Contents[0])[0]);

  // Add the attribute names to the PP
  if(Rec2Add.Contents.size()>1){
    AttrFilter=BinDataSetAdd(AttrFilter,Rec2Add.Contents[2]);
    ToUpdate.Contents[2]=AttrFilter;    
  }

  ToUpdate.Contents[1]=RecFilter;

  vector<Amoeba_Rec> AmoebaData=WMS2Amoeba(ToUpdate);
  WriteAll2DB(AmoebaData);
}

void WMSAPI::SetPP(BinData PPName){
  BinData BinRecName(":pprofiles");
  unsigned long int Pos=Search(BinRecName);
  WMS_Rec ToUpdate={};
  if(Pos==-1){
    vector<BinData> Header={};
    vector<BinData> Attributes={};
    vector<BinData> Values={};
    Header.push_back(BinRecName);
    Attributes.push_back(PPName);
    Values.push_back(PPName);
    ToUpdate.Contents.push_back(Header);
    ToUpdate.Contents.push_back(Attributes);
    ToUpdate.Contents.push_back(Values);
  }
  else{
    ToUpdate=Amoeba2WMS(Bin2Amoeba(GetData(Pos)));
    if((ToUpdate.Contents).size()<3)
      (ToUpdate.Contents).resize(3);
    ToUpdate.Contents[1]=BinDataSetAdd(ToUpdate.Contents[1],PPName);
    ToUpdate.Contents[2]=BinDataSetAdd(ToUpdate.Contents[2],PPName);
  }
  vector<Amoeba_Rec> AmoebaData=WMS2Amoeba(ToUpdate);
  WriteAll2DB(AmoebaData);
  
  /* Note that the PP record itself will be created because records and attributes are the same. */  

  ActivePP=PPName;
  PopulateFilter(PPName);
}

void WMSAPI::SetPP(string PPName){
  BinData SearchToken(PPName);
  SetPP(SearchToken);
}

vector<string> WMSAPI::GetSegment(BinData RecName,unsigned long int Seg){
  vector<string> List={};  
  unsigned long int Pos=Search(RecName);
  if(Pos!=-1){
    vector<BinData> BinProfiles=Amoeba2WMS(Bin2Amoeba(GetData(Pos))).Contents[Seg];
    for(size_t _=0;_<BinProfiles.size();_++)
      List.push_back(string(BinProfiles[_].Str,BinProfiles[_].Size));
  }
  return List;
}

vector<string> WMSAPI::GetSegment(string RecName,unsigned long int Seg){
  BinData SearchToken(RecName);
  return GetSegment(SearchToken,Seg);
}

vector<string> WMSAPI::GetPPList(){
  return GetSegment(":pprofiles",1);
}

unsigned long int WMSAPI::GetAttrRef(unsigned long int RefId,BinData DataPrefix){
  unsigned long int C=0,AttrRef=-1;
  // Retrieve the Amoeba record at RefId
  if(RefId==-1)
    return -1;
  Amoeba_Rec NewRec=Bin2Amoeba(GetData(RefId));
  // We check if we have the segments we need
  if(NewRec.RefSeg.size()!=4)
    return -1;
  // Now we go through the attribute names
  for(C=0;C<(NewRec.RefSeg[2]).size();C++){
    BinData AttrName=GetName((NewRec.RefSeg[2])[C]);
    if(AttrName.Size<DataPrefix.Size)
      continue;
    if(!memcmp(AttrName.Str,DataPrefix.Str,DataPrefix.Size)){
      AttrRef=C;
      break;
    }
  }
  return AttrRef;
}

Amoeba_Rec WMSAPI::FillWithBlankSegments(Amoeba_Rec Amoeba_Record){
  if(Amoeba_Record.RefSeg.size()<4){
    unsigned long int PreviousSegments=Amoeba_Record.RefSeg.size();
    Amoeba_Record.RefSeg.resize(4);
    for(;PreviousSegments<4;PreviousSegments++)
      Amoeba_Record.RefSeg[PreviousSegments]={};
  }
  return Amoeba_Record;
}

void WMSAPI::AddEmptyAmoebaRec(BinData Name){
  unsigned long int RecordLocation=Search(Name);  
  if(RecordLocation==-1){
    Amoeba_Rec NewRec;
    NewRec=Name;
    
    AddData(Amoeba2Bin(NewRec));
  }
}

void WMSAPI::AddEmptyAmoebaRec(string Name){
  BinData BinName=Name;
  AddEmptyAmoebaRec(BinName);
}

bool WMSAPI::RepWMSAttr(BinData ReplaceWith,unsigned long int RefId,unsigned long int SegId,unsigned long int AttrId){
  /* Locking needs to occur around the entire contents of this function */
  unsigned long int OldTarget=0,NewTarget=0,ShadowedRecordNum=RecordNum;

  if(RefId==-1)
    return false;

  /* Necessary because Amoeba records reserve the first segment for inbound references */
  SegId++;

  Amoeba_Rec Amoeba_Record=Bin2Amoeba(GetData(RefId));
  vector<Amoeba_Rec> ToApply={};
  
  // I can't remember if / why this is required.
  if(SegId==1){
    AttrId--;
  }

  if((SegId==1)&&(AttrId==-1)){
    Amoeba_Record.PrimaryKey=ReplaceWith;

    /* Need to handle the situation where the record is renamed to be the same as another record name. */
  }
  else{
    OldTarget=(Amoeba_Record.RefSeg[SegId])[AttrId];
    Amoeba_Rec OldTarget_Rec=Bin2Amoeba(GetData(OldTarget));  

    /* Modify the new attribute first */
    NewTarget=Search(ReplaceWith);
    Amoeba_Rec NewTarget_Rec={};
    if(NewTarget==-1){
      //      NewTarget_Rec.PrimaryKey=ReplaceWith;

      NewTarget_Rec=NewAmoeba(ReplaceWith);

      NewTarget=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(ShadowedRecordNum*sizeof(unsigned long int)));
      ShadowedRecordNum++;
    }
    else{
      NewTarget_Rec=Bin2Amoeba(GetData(NewTarget));
    }
    NewTarget_Rec.Location=NewTarget;
    NewTarget_Rec.Target=false;
    (NewTarget_Rec.RefSeg[0]).push_back(RefId);
    (NewTarget_Rec.RefSeg[0]).push_back(SegId);
    (NewTarget_Rec.RefSeg[0]).push_back(AttrId);
    ToApply.push_back(NewTarget_Rec);

    /* Delink the old attribute */
    OldTarget_Rec=Delink(OldTarget_Rec,RefId,SegId,AttrId);
    OldTarget_Rec.Location=OldTarget;
    OldTarget_Rec.Target=false;
    ToApply.push_back(OldTarget_Rec);  

    /* Modify the record to point to the new attribute away from the old */
    if((Amoeba_Record.RefSeg.size()>SegId)&&(Amoeba_Record.RefSeg[SegId].size()>AttrId))
      Amoeba_Record.RefSeg[SegId][AttrId]=NewTarget;
    else
      return false;
  }
  Amoeba_Record.Location=RefId;
  Amoeba_Record.Target=false;
  ToApply.push_back(Amoeba_Record);
  WriteAll2DB(ToApply);
  return true;
}

bool WMSAPI::DelWMSPair(unsigned long int RefId,BinData AttrName){
  /* Locking needs to occur around the entire contents of this function */
  bool AttrFound=false;
  unsigned long int AttrTarget=0,ValueTarget=0,AttrId=0,C=0;
  if(RefId==-1)
    return false;
  Amoeba_Rec Amoeba_Record=Bin2Amoeba(GetData(RefId));
  Amoeba_Record.Location=RefId;
  Amoeba_Record.Target=false;  
  vector<Amoeba_Rec> ToApply={};

  Amoeba_Record=FillWithBlankSegments(Amoeba_Record);

  // Need to work out what AttrId is!

  // loop though RefSeg[2]
  for(C=0;C<(Amoeba_Record.RefSeg[2]).size();C++){
    AttrId=(Amoeba_Record.RefSeg[2])[C];
    if(GetName(AttrId)==AttrName){
      AttrFound=true;
      break;
    }
  }

  if(!AttrFound)
    return false;

  /* Need the attribute value pair for attrid, which are segments 2 and 3 */
  if((Amoeba_Record.RefSeg.size()>3)&&(Amoeba_Record.RefSeg[2].size()>AttrId)&&(Amoeba_Record.RefSeg[3].size()>AttrId)){
    AttrTarget=Amoeba_Record.RefSeg[2][AttrId];
    ValueTarget=Amoeba_Record.RefSeg[3][AttrId];    
    if((AttrTarget<0)&&(ValueTarget<0))
      return false;
    Amoeba_Rec Amoeba_Attr=Bin2Amoeba(GetData(AttrTarget));
    Amoeba_Rec Amoeba_Value=Bin2Amoeba(GetData(ValueTarget));
    Amoeba_Attr=Delink(Amoeba_Attr,RefId);
    Amoeba_Attr.Location=AttrTarget;
    Amoeba_Attr.Target=false;
    Amoeba_Value=Delink(Amoeba_Value,RefId);
    Amoeba_Value.Location=ValueTarget;
    Amoeba_Value.Target=false;
    Amoeba_Record.RefSeg[2].erase(Amoeba_Record.RefSeg[2].begin()+AttrId,Amoeba_Record.RefSeg[2].begin()+AttrId+1);
    Amoeba_Record.RefSeg[3].erase(Amoeba_Record.RefSeg[3].begin()+AttrId,Amoeba_Record.RefSeg[3].begin()+AttrId+1);
    ToApply.push_back(Amoeba_Record);
    ToApply.push_back(Amoeba_Attr);
    ToApply.push_back(Amoeba_Value);
    WriteAll2DB(ToApply);
  }
  else
    return false;
  return true;
}

bool WMSAPI::DelWMSPair(unsigned long int RefId,unsigned long int AttrId){
  /* Locking needs to occur around the entire contents of this function */
  unsigned long int AttrTarget=0,ValueTarget=0;
  if(RefId==-1)
    return false;
  Amoeba_Rec Amoeba_Record=Bin2Amoeba(GetData(RefId));
  Amoeba_Record.Location=RefId;
  Amoeba_Record.Target=false;  
  vector<Amoeba_Rec> ToApply={};
  /* Need the attribute value pair for attrid, which are segments 2 and 3 */
  if((Amoeba_Record.RefSeg.size()>3)&&(Amoeba_Record.RefSeg[2].size()>AttrId)&&(Amoeba_Record.RefSeg[3].size()>AttrId)){
    AttrTarget=Amoeba_Record.RefSeg[2][AttrId];
    ValueTarget=Amoeba_Record.RefSeg[3][AttrId];    
    if((AttrTarget<0)&&(ValueTarget<0))
      return false;
    Amoeba_Rec Amoeba_Attr=Bin2Amoeba(GetData(AttrTarget));
    Amoeba_Rec Amoeba_Value=Bin2Amoeba(GetData(ValueTarget));
    Amoeba_Attr=Delink(Amoeba_Attr,RefId);
    Amoeba_Attr.Location=AttrTarget;
    Amoeba_Attr.Target=false;
    Amoeba_Value=Delink(Amoeba_Value,RefId);
    Amoeba_Value.Location=ValueTarget;
    Amoeba_Value.Target=false;
    Amoeba_Record.RefSeg[2].erase(Amoeba_Record.RefSeg[2].begin()+AttrId,Amoeba_Record.RefSeg[2].begin()+AttrId+1);
    Amoeba_Record.RefSeg[3].erase(Amoeba_Record.RefSeg[3].begin()+AttrId,Amoeba_Record.RefSeg[3].begin()+AttrId+1);
    ToApply.push_back(Amoeba_Record);
    ToApply.push_back(Amoeba_Attr);
    ToApply.push_back(Amoeba_Value);
    WriteAll2DB(ToApply);
  }
  else
    return false;
  return true;
}

bool WMSAPI::MergeFilteredWMSPair(BinData AddAttr,BinData AddValue,unsigned long int RefId){
  bool Returned=AddWMSPair(AddAttr,AddValue,RefId);
  WMS_Rec Merged=Amoeba2WMS(Bin2Amoeba(GetData(RefId)));
  AddRec2PP(Merged);
  return Returned;
}

bool WMSAPI::AddWMSPair(BinData AddAttr,BinData AddValue,unsigned long int RefId){
  /* Locking needs to occur around the entire contents of this function */
  bool DuplicateAttr=false;
  unsigned long int AttrTarget=0,ValueTarget=0,ShadowedRecordNum=RecordNum,AttrId=0;

  if(RefId==-1)
    return false;

  Amoeba_Rec Amoeba_Record=Bin2Amoeba(GetData(RefId));
  Amoeba_Record=FillWithBlankSegments(Amoeba_Record);

  AttrId=Amoeba_Record.RefSeg[2].size();
  if(AttrId!=Amoeba_Record.RefSeg[3].size())
    return false;

  Amoeba_Record.Location=RefId;
  Amoeba_Record.Target=false;
  Amoeba_Rec Amoeba_Attr={};
  Amoeba_Rec Amoeba_Value={};
  vector<Amoeba_Rec> ToApply={};  

  /* Need to see if the records exist */

  AttrTarget=Search(AddAttr);   // ARE WE RETURNING THE ACTUAL STATIC POSITION OF THE RECORD
  ValueTarget=Search(AddValue); // ARE WE RETURNING THE ACTUAL STATIC POSITION OF THE RECORD

  if(AttrTarget==-1){
    Amoeba_Attr=NewAmoeba(AddAttr);
    AttrTarget=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+ShadowedRecordNum*sizeof(unsigned long int));
    ShadowedRecordNum++;
  }
  else{
    Amoeba_Attr=Bin2Amoeba(GetData(AttrTarget));
  }

  if(AddAttr==AddValue){
    DuplicateAttr=true;
    ValueTarget=AttrTarget;
  }
  else{
    if(ValueTarget==-1){
      Amoeba_Value=NewAmoeba(AddValue);
      ValueTarget=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+ShadowedRecordNum*sizeof(unsigned long int));
      ShadowedRecordNum++;
    }
    else{
      Amoeba_Value=Bin2Amoeba(GetData(ValueTarget));
    }
  }

  // Add the attribute pair  
  Amoeba_Record.RefSeg[2].push_back(AttrTarget);
  Amoeba_Record.RefSeg[3].push_back(ValueTarget);
  ToApply.push_back(Amoeba_Record);

  // Add the inbound references, Attr is always added
  Amoeba_Attr.Target=false;
  Amoeba_Attr.Location=AttrTarget;
  Amoeba_Attr.RefSeg[0].push_back(RefId);
  Amoeba_Attr.RefSeg[0].push_back(2);
  Amoeba_Attr.RefSeg[0].push_back(AttrId);

  if(DuplicateAttr){
    // Then we add it again (value is the same as attribute) just different segment id.
    
    Amoeba_Attr.RefSeg[0].push_back(RefId);
    Amoeba_Attr.RefSeg[0].push_back(3);
    Amoeba_Attr.RefSeg[0].push_back(AttrId);
  }
  else{
    // Otherwise we add the value

    Amoeba_Value.Target=false;
    Amoeba_Value.Location=ValueTarget;
    Amoeba_Value.RefSeg[0].push_back(RefId);
    Amoeba_Value.RefSeg[0].push_back(3);
    Amoeba_Value.RefSeg[0].push_back(AttrId);    
  }
  
  ToApply.push_back(Amoeba_Attr);
  if(!DuplicateAttr)
    ToApply.push_back(Amoeba_Value);
  WriteAll2DB(ToApply);
  return true;
}

void WMSAPI::AddWMSToDB(WMS_Rec ToAdd){
  vector<Amoeba_Rec> Data=WMS2Amoeba(ToAdd);
  WriteAll2DB(Data);
  //  AddRec2PP(ToAdd);
}

bool WMSAPI::DelWMSInDB(string ToDelete){
  BinData PrimaryKey(ToDelete.c_str(),ToDelete.size());
  return DelWMSInDB(PrimaryKey);
}

bool WMSAPI::DelWMSInDB(BinData ToDelete){
  /* Locking needs to occur around the entire contents of this function */
  unsigned long int TargetLoc=0,SegIndex=0,AttribIndex=0,AttribLoc=0;
  Amoeba_Rec Amoeba_Record={};
  vector<Amoeba_Rec> AttribList={};
  TargetLoc=Search(ToDelete);                                        // Find the record
  if(TargetLoc==-1)                                                  // If it doesn't exist, exit doing nothing
    return false;
  Amoeba_Record=Bin2Amoeba(GetData(TargetLoc));                      // Need to get the Amoeba record, not the WMS conversion
  for(SegIndex=0;SegIndex<Amoeba_Record.RefSeg.size();SegIndex++){   // We need to loop through all the attributes and extract the Amoeba attributes
    for(AttribIndex=0;AttribIndex<(Amoeba_Record.RefSeg[SegIndex]).size();AttribIndex++){
      AttribLoc=(Amoeba_Record.RefSeg[SegIndex])[AttribIndex];
      if(AttribLoc==-1)                                              // The attribute reference is broken for some reason, skip to the next attr
	continue;
      Amoeba_Rec ScratchBuffer=Delink(Bin2Amoeba(GetData(AttribLoc)),TargetLoc);
      ScratchBuffer.Target=false;
      ScratchBuffer.Location=AttribLoc;
      AttribList.push_back(ScratchBuffer);
    }
  }
  WriteAll2DB(AttribList);                                           // Write the delinked records back to the database
  DelData(TargetLoc);                                                // Delete the original record
  return true;
}

bool WMSAPI::DelWMSInDB(unsigned long int Pos){
  /* Locking needs to occur around the entire contents of this function */
  unsigned long int TargetLoc=0,SegIndex=0,AttribIndex=0,AttribLoc=0;
  Amoeba_Rec Amoeba_Record={};
  vector<Amoeba_Rec> AttribList={};
  /*
  TargetLoc=Search(ToDelete);                                        // Find the record
  if(TargetLoc==-1)                                                  // If it doesn't exist, exit doing nothing
    return false;
  */
  TargetLoc=Pos;
  Amoeba_Record=Bin2Amoeba(GetData(TargetLoc));                      // Need to get the Amoeba record, not the WMS conversion
  for(SegIndex=0;SegIndex<Amoeba_Record.RefSeg.size();SegIndex++){   // We need to loop through all the attributes and extract the Amoeba attributes
    for(AttribIndex=0;AttribIndex<(Amoeba_Record.RefSeg[SegIndex]).size();AttribIndex++){
      AttribLoc=(Amoeba_Record.RefSeg[SegIndex])[AttribIndex];
      if(AttribLoc==-1)                                              // The attribute reference is broken for some reason, skip to the next attr
	continue;
      Amoeba_Rec ScratchBuffer=Delink(Bin2Amoeba(GetData(AttribLoc)),TargetLoc);
      ScratchBuffer.Target=false;
      ScratchBuffer.Location=AttribLoc;
      AttribList.push_back(ScratchBuffer);
    }
  }
  WriteAll2DB(AttribList);                                           // Write the delinked records back to the database
  DelData(TargetLoc);                                                // Delete the original record
  return true;
}

void WMSAPI::ShowAllAmoebas(string Prefix){
  unsigned long int C=0,StaticIndex=0;
  /* Go through the search table */
  for(C=0;C<RecordNum;C++){
    Amoeba_Rec Amoeba_Record={};
    StaticIndex=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+C*sizeof(unsigned long int));
    Amoeba_Record=Bin2Amoeba(GetData(StaticIndex));
    Amoeba_Record.Location=StaticIndex;
    putchar('\n');
    Amoeba_Record.Print(Prefix);
  }
  putchar('\n');
}

void WMSAPI::ShowAllWMS(string Prefix){
  unsigned long int C=0,StaticIndex=0;
  /* Go through the search table */
  for(C=0;C<RecordNum;C++){    
    Amoeba_Rec Amoeba_Record={};
    StaticIndex=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+C*sizeof(unsigned long int));
    Amoeba_Record=Bin2Amoeba(GetData(StaticIndex));
    Amoeba_Record.Location=StaticIndex;
    WMS_Rec Converted=Amoeba2WMS(Amoeba_Record);
    PrintWMSRec(Prefix,Converted);
  }
}

void WMSAPI::ShowAllAmoebas(){
  ShowAllAmoebas("");
}

void WMSAPI::ShowAllWMS(){
  ShowAllWMS("");
}

Amoeba_Rec &Amoeba_Rec::operator=(const string &Data){
  this->PrimaryKey=Data;
  RefSeg.resize(4);
  RefSeg[0]={};
  RefSeg[1]={};
  RefSeg[2]={};
  RefSeg[3]={};
}

Amoeba_Rec &Amoeba_Rec::operator=(const BinData &Data){
  this->PrimaryKey=Data;
  RefSeg.resize(4);
  RefSeg[0]={};
  RefSeg[1]={};
  RefSeg[2]={};
  RefSeg[3]={};
}

Amoeba_Rec Amoeba_Rec::Add2Segment(class Amoeba_Rec Source,unsigned long int SegId,unsigned long int Pos){
  Source.RefSeg[SegId].push_back(Pos);
  return Source;
}

Amoeba_Rec Amoeba_Rec::AddSegment(class Amoeba_Rec Source,vector<unsigned long int> Seg){
  Source.RefSeg.push_back(Seg);
  return Source;
}

Amoeba_Rec Amoeba_Rec::DelSegment(class Amoeba_Rec Source,unsigned long int DelSeg){
  Source.RefSeg.erase(Source.RefSeg.begin()+DelSeg);
  return Source;
}

Amoeba_Rec Amoeba_Rec::RepSegment(class Amoeba_Rec Source,vector<unsigned long int> Seg,unsigned long int RepSeg){
  Source.RefSeg[RepSeg]=Seg;
  return Source;
}

WMS_Rec WMSAPI::Amoeba2WMS(Amoeba_Rec ToConv){
  unsigned long int SegmentId=0,Pos=0;
  vector<BinData> ThisSegment={};
  WMS_Rec Original={};

  // should always contain a header segment
  ThisSegment.push_back(ToConv.PrimaryKey);
  for(SegmentId=1;SegmentId<ToConv.RefSeg.size();SegmentId++){
    vector<unsigned long int> *NewRefSeg=&(ToConv.RefSeg[SegmentId]);
    for(Pos=0;Pos<NewRefSeg->size();Pos++){
      ThisSegment.push_back(GetName((*NewRefSeg)[Pos]));
    }
    Original=AddSeg2WMSRec(Original,ThisSegment);
    ThisSegment={};
  }
  return Original;
}

Amoeba_Rec WMSAPI::Delink(Amoeba_Rec Inp,unsigned long int RefId){
  int A=0,B=0;
  if(Inp.RefSeg.size()){
    for(int A=0;A<Inp.RefSeg[0].size();A+=3)
      if((Inp.RefSeg[0])[A]==RefId){
	Inp.RefSeg[0].erase(Inp.RefSeg[0].begin(),Inp.RefSeg[0].begin()+3);
	A-=3;
      }
  }
  return Inp;
}

Amoeba_Rec WMSAPI::Delink(Amoeba_Rec Inp,unsigned long int RefId,unsigned long int SegId,unsigned long int AttrId){
  int A=0,B=0;
  if(Inp.RefSeg.size()){
    for(int A=0;A<Inp.RefSeg[0].size();A+=3)
      if(((Inp.RefSeg[0])[A]==RefId)&&((Inp.RefSeg[0])[A+1]==SegId)&&((Inp.RefSeg[0])[A+2]==AttrId)){
	Inp.RefSeg[0].erase(Inp.RefSeg[0].begin(),Inp.RefSeg[0].begin()+3);
	A-=3;
      }
  }
  return Inp;
}

vector<Amoeba_Rec> WMSAPI::WMS2Amoeba(WMS_Rec ToConv){
  // Needs to be locked around ths entire function.
  bool Found=false;
  unsigned long int A=0,B=0,C=0,FoundLoc=0,ShadowedRecordNum=RecordNum,AttributeIndex=0,TargetSeg=0,TargetPos=0,AttributeLoc=0;
  BinData *Ref,Retrieved={};

  vector <BinData *> UniqueName={};
  vector <unsigned long int> UniqueLoc={};
  vector <Amoeba_Rec> UniqueRec={};
  vector <unsigned long int> OutboundReferences={};

  BinData *TargetName=NULL;
  unsigned long int TargetLoc=0;
  Amoeba_Rec TargetRec={};
  Amoeba_Rec FoundRec={};

  // get the first record as it will be handled differently. 
  if(ToConv.Contents.size()&&ToConv.Contents[0].size()){
    TargetName=&((ToConv.Contents[0])[0]);
    TargetLoc=Search(*TargetName);
    Amoeba_Rec TargetRec={};
    if(TargetLoc==-1){
      TargetRec=NewAmoeba(*TargetName);
      TargetLoc=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(ShadowedRecordNum*sizeof(unsigned long int)));
      ShadowedRecordNum++;
    }
    else{
      /* this is the problem i think */
      TargetRec=Bin2Amoeba(GetData(TargetLoc));
      vector<unsigned long int> Header=TargetRec.RefSeg[0];
      TargetRec.RefSeg.resize(4);
      TargetRec.RefSeg[0]=Header;
      
      TargetRec.RefSeg[1]={};
      TargetRec.RefSeg[2]={};
      TargetRec.RefSeg[3]={};
      
    }

    TargetRec.PrimaryKey=*TargetName;
    TargetRec.Location=TargetLoc;
    TargetRec.Target=true;

    vector<BinData> Header=ToConv.Contents[0];
    Header.erase(Header.begin(),Header.begin()+1);
    ToConv.Contents[0]=Header;

    // Lets start by making a unique list of elements in the WMS record
    for(A=0;A<ToConv.Contents.size();A++){
      OutboundReferences={};
      TargetSeg=A+1;
      for(B=0;B<(ToConv.Contents[A]).size();B++){
	TargetPos=B;
	Ref=&((ToConv.Contents[A])[B]);
	Found=false;
	for(C=0;C<UniqueName.size();C++){
	  if(*(UniqueName[C])==*(Ref)){
	    Found=true;
	    AttributeIndex=C;
	    break;
	  }
	}
	if(!Found){
	  UniqueName.push_back(Ref);
	  FoundLoc=Search(*Ref);
	  Amoeba_Rec ThisAmoeba={};
	  if(FoundLoc==-1){
	    ThisAmoeba=NewAmoeba(*Ref);
	    FoundLoc=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(ShadowedRecordNum*sizeof(unsigned long int)));
	    ShadowedRecordNum++;
	  }
	  else{
	    ThisAmoeba=Bin2Amoeba(GetData(FoundLoc));
	  }
	  ThisAmoeba.Location=FoundLoc;
	  ThisAmoeba.Target=false;
	  AttributeIndex=UniqueLoc.size();
	  UniqueLoc.push_back(FoundLoc);
	  UniqueRec.push_back(ThisAmoeba);
	}
	AttributeLoc=UniqueLoc[AttributeIndex];

	// Add the outbound reference to the attribute from the target record to the record reference at attribute index.
	OutboundReferences.push_back(AttributeLoc);

	// Insert the inbound reference from the TargetLoc / TargetSeg / TargetPos into the attribute records.	
	if(UniqueRec[AttributeIndex].RefSeg.size()==0){
	  vector <unsigned long int> InboundReferences={};
	  (UniqueRec[AttributeIndex]).RefSeg.push_back(InboundReferences);
	}
	UniqueRec[AttributeIndex]=Delink(UniqueRec[AttributeIndex],TargetLoc);
	(UniqueRec[AttributeIndex]).RefSeg[0].push_back(TargetLoc);
	(UniqueRec[AttributeIndex]).RefSeg[0].push_back(TargetSeg);
	(UniqueRec[AttributeIndex]).RefSeg[0].push_back(TargetPos);
      }
      TargetRec.RefSeg[TargetSeg]=OutboundReferences;
    }
    UniqueRec.insert(UniqueRec.begin(),TargetRec);
  }
  return UniqueRec;
}

void WMSAPI::DebugAll(vector <Amoeba_Rec> Inp){
  for(int C=0;C<Inp.size();C++)
    Inp[C].Print();
}

void WMSAPI::WriteAll2DB(vector <Amoeba_Rec> Inp){
  for(int C=0;C<Inp.size();C++){
    RepData(Amoeba2Bin(Inp[C]),Inp[C].Location);
  }
}


void Amoeba_Rec::Print(string Prefix){
  unsigned long int Segment=0,Index=0;
  const char *CharPrefix=Prefix.c_str();
  printf("%s=[",CharPrefix);
  PrimaryKey.Print();
  printf("[%lu]",(unsigned long int)PrimaryKey.Size);
  puts("]=====================");  
  printf("%sPosition:%lu Target:%d\n",CharPrefix,(unsigned long int)Location,Target);
  for(Segment=0;Segment<RefSeg.size();Segment++){
    printf("%s-[Segment: %03ld]------\n",CharPrefix,(unsigned long int)Segment);
    printf("%s",CharPrefix);
    for(Index=0;Index<(RefSeg[Segment].size());Index++){
      printf("[%ld]",(unsigned long int)(RefSeg[Segment])[Index]);
    }
    putchar('\n');
  }
  printf("%s=================================\n",CharPrefix);
  
}

void Amoeba_Rec::Print(){
  Print("");
}

Amoeba_Rec WMSAPI::NewAmoeba(BinData Name){
  Amoeba_Rec NewAmoeba={};
  NewAmoeba.PrimaryKey=Name;
  NewAmoeba.RefSeg.push_back({});
  NewAmoeba.RefSeg.push_back({});
  NewAmoeba.RefSeg.push_back({});
  NewAmoeba.RefSeg.push_back({});
  return NewAmoeba;
}

BinData WMSAPI::Amoeba2Bin(struct Amoeba_Rec ToConv){
  unsigned long int SizeofObject=0,_=0,C=0,NumofSegs=0,TotalSize=0,SegmentSize=0,Ref=0;
  char *DataBuf=NULL;

  // number of segments in the record
  SizeofObject+=sizeof(unsigned long int);

  // number of elements that would be in the WMS record [but isn't in Amoeba], then the size of the name
  SizeofObject+=2*sizeof(unsigned long int);
  SizeofObject+=ToConv.PrimaryKey.Size;

  //number of elements in the vector
  NumofSegs=ToConv.RefSeg.size();
  for(_=0;_<NumofSegs;_++){
    SizeofObject+=sizeof(unsigned long int);
    for(C=0;C<ToConv.RefSeg[_].size();C++)
      SizeofObject+=sizeof(unsigned long int);
  }
  
  DataBuf=(char *)malloc(SizeofObject*sizeof(char));
  memcpy(DataBuf+TotalSize,&NumofSegs,sizeof(unsigned long int));
  TotalSize+=2*sizeof(unsigned long int);

  memcpy(DataBuf+TotalSize,&(ToConv.PrimaryKey.Size),sizeof(unsigned long int));
  TotalSize+=sizeof(unsigned long int);
  
  memcpy(DataBuf+TotalSize,ToConv.PrimaryKey.Str,ToConv.PrimaryKey.Size);
  TotalSize+=ToConv.PrimaryKey.Size;

  for(_=0;_<NumofSegs;_++){
    SegmentSize=ToConv.RefSeg[_].size();
    memcpy(DataBuf+TotalSize,&SegmentSize,sizeof(unsigned long int));
    TotalSize+=sizeof(unsigned long int);
    for(C=0;C<SegmentSize;C++){
      Ref=(ToConv.RefSeg[_])[C];
      memcpy(DataBuf+TotalSize,&Ref,sizeof(unsigned long int));
      TotalSize+=sizeof(unsigned long int);
    }
  }
  BinData NewData(DataBuf,TotalSize);  
  return NewData;
}

struct Amoeba_Rec WMSAPI::Bin2Amoeba(BinData ToConv){
  unsigned long int NumSegments=0,Offset=0,SizeofPrimaryKey=0,_=0,C=0,SizeofSeg=0,TheReference=0;
  char *PrimaryKey=NULL;
  vector<vector<unsigned long int>> References;
  vector<unsigned long int> Segment;

  Amoeba_Rec AmoebaData={};

  NumSegments=*(unsigned long int *)(ToConv.Str+Offset);
  Offset+=2*(sizeof(unsigned long int));

  SizeofPrimaryKey=*(unsigned long int *)(ToConv.Str+Offset);
  Offset+=sizeof(unsigned long int);
  PrimaryKey=(char *)malloc((SizeofPrimaryKey+1)*sizeof(char));
  memcpy(PrimaryKey,ToConv.Str+Offset,SizeofPrimaryKey);
  
  PrimaryKey[SizeofPrimaryKey]=0;
  Offset+=SizeofPrimaryKey;
  
  BinData NewData(PrimaryKey,SizeofPrimaryKey);
  AmoebaData.PrimaryKey=NewData;
  
  References.resize(0);
  for(_=0;_<NumSegments;_++){
    SizeofSeg=*(unsigned long int *)(ToConv.Str+Offset);
    Offset+=sizeof(unsigned long int);
    Segment.resize(0);
    for(C=0;C<SizeofSeg;C++){
      TheReference=*(unsigned long int *)(ToConv.Str+Offset);
      Offset+=sizeof(unsigned long int);
      Segment.push_back(TheReference);
    }
    References.push_back(Segment);
  }
  AmoebaData.RefSeg=References;
  return AmoebaData;
}

WMS_Rec WMSAPI::AddSeg2WMSRec(WMS_Rec Original,vector<BinData> Segment){
  Original.Contents.push_back(Segment);
  return Original;
}

WMS_Rec WMSAPI::AddSeg2WMSRec(WMS_Rec Original,vector<string> Segment){
  unsigned long int _=0;
  vector<BinData> VecOfBinData={};
  for(_=0;_<Segment.size();_++){
    BinData String2BinData(Segment[_]); 
    VecOfBinData.push_back(String2BinData);
  }
  Original.Contents.push_back(VecOfBinData);
  return Original;
}



WMS_Rec WMSConversion::Binary2WMS(BinData Data){
  unsigned long int NumofSegments=0,Offset=0,SegId=0,AttrId=0,NumofAttrs=0,AttrSize=0;
  char *AttrData=NULL;
  vector<BinData> Segment;
  WMS_Rec NewWMSRec={};
  NewWMSRec.Contents.resize(0);
  if(Data.Size){
    NumofSegments=*(unsigned long int *)(Data.Str+Offset);
    Offset+=sizeof(unsigned long int);
    for(SegId=0;SegId<NumofSegments;SegId++){
      Segment.resize(0);
      NumofAttrs=*(unsigned long int *)(Data.Str+Offset);
      Offset+=sizeof(unsigned long int);
      for(AttrId=0;AttrId<NumofAttrs;AttrId++){
	AttrSize=*(unsigned long int *)(Data.Str+Offset);
	Offset+=sizeof(unsigned long int);
	AttrData=(char *)(Data.Str+Offset);
	Offset+=AttrSize;
	BinData NewData(AttrData,AttrSize);
	Segment.push_back(NewData);
      }
      NewWMSRec.Contents.push_back(Segment);      
    }
  }
  return NewWMSRec;
}

BinData WMSConversion::WMS2Binary(WMS_Rec Rec){
  unsigned long int TotalSize=0,Segment=0,AttrNum=0,NumofSegs=0,NumofAttrs=0;
  char *DataBuf=NULL;
  vector<BinData> Temp;
  /* Calculate the size of the record */
  TotalSize=sizeof(unsigned long int);
  NumofSegs=Rec.Contents.size();
  for(Segment=0;Segment<NumofSegs;Segment++){
    Temp=Rec.Contents[Segment];
    TotalSize+=sizeof(unsigned long int);
    NumofAttrs=Temp.size();
    for(AttrNum=0;AttrNum<NumofAttrs;AttrNum++){
      TotalSize+=sizeof(unsigned long int);
      TotalSize+=Temp[AttrNum].Size;
    }
  }
  DataBuf=(char *)malloc((TotalSize)*sizeof(char));
  /* Write the data */
  //  memset(DataBuf,0,TotalSize+1); 
  TotalSize=0;
  /* Number of segments */
  NumofSegs=Rec.Contents.size();
  memcpy(DataBuf+TotalSize,&NumofSegs,sizeof(unsigned long int));
  TotalSize=sizeof(unsigned long int);
  NumofSegs=Rec.Contents.size();
  for(Segment=0;Segment<NumofSegs;Segment++){
    Temp=Rec.Contents[Segment];
    /* Number of elements in the segment */
    NumofAttrs=Temp.size();
    memcpy(DataBuf+TotalSize,&NumofAttrs,sizeof(unsigned long int));
    TotalSize+=sizeof(unsigned long int);
    for(AttrNum=0;AttrNum<NumofAttrs;AttrNum++){
      /* Size of the data area */
      memcpy(DataBuf+TotalSize,&(Temp[AttrNum].Size),sizeof(unsigned long int));
      TotalSize+=sizeof(unsigned long int);
      /* The data itself */
      memcpy(DataBuf+TotalSize,Temp[AttrNum].Str,Temp[AttrNum].Size);
      TotalSize+=Temp[AttrNum].Size;
    }
  }
  BinData NewData(DataBuf,TotalSize);  
  return NewData;
}


void WMSConversion::PrintWMSRec(WMS_Rec Rec){
  PrintWMSRec("",Rec);
}

void WMSConversion::PrintWMSRec(string Prefix,WMS_Rec Rec){
  unsigned long int Size1=Rec.Contents.size(),Size2=0,Pos1=0,Pos2=0;
  printf("%s=[WMS_REC]===================================\n",Prefix.c_str());
  for(Pos1=0;Pos1<Size1;Pos1++){
    Size2=Rec.Contents[Pos1].size();
    printf("%s<%lu>:",Prefix.c_str(),(unsigned long int)Pos1);
    for(Pos2=0;Pos2<Size2;Pos2++){
      putchar('[');
      Rec.Contents[Pos1][Pos2].Print();
      putchar(']');
    }
    putchar('\n');
  }
  printf("%s=============================================\n",Prefix.c_str());
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
