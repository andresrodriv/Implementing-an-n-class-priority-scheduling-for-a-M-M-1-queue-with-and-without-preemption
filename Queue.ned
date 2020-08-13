//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple Queue
{
    parameters:
        volatile double serviceTimeclass1 @unit(s);
        volatile double serviceTimeclass2 @unit(s);
        volatile double serviceTimeclass3 @unit(s);
        volatile double serviceTimeclass4 @unit(s);
        volatile double serviceTimeclass5 @unit(s);
        int policy;
        @display("i=block/queue;q=queue");
        //Defoult
        @signal[qlen](type="long");
        @signal[busy](type="bool"); 
        @statistic[qlen](title="queue length";record=vector,mean;interpolationmode=sample-hold);
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
        //Global
        @signal[queueingTime](type="simtime_t");//waiting time in the queue
        @signal[responseTime](type="simtime_t");
        @statistic[queueingTime](title="queueing time at the queue";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[responseTime](title="response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        //Per Class
        @signal[queueingTimeclass1](type="simtime_t");//waiting time in the queue
        @signal[queueingTimeclass2](type="simtime_t");
        @signal[queueingTimeclass3](type="simtime_t");
        @signal[queueingTimeclass4](type="simtime_t");
        @signal[queueingTimeclass5](type="simtime_t");
        @statistic[queueingTimeclass1](title="queueing time at the queue";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[queueingTimeclass2](title="queueing time at the queue";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[queueingTimeclass3](title="queueing time at the queue";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[queueingTimeclass4](title="queueing time at the queue";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[queueingTimeclass5](title="queueing time at the queue";unit=s;record=vector,mean;interpolationmode=none);
        
        @signal[responseTimeclass1](type="simtime_t");
        @signal[responseTimeclass2](type="simtime_t");
        @signal[responseTimeclass3](type="simtime_t");
        @signal[responseTimeclass4](type="simtime_t");
        @signal[responseTimeclass5](type="simtime_t");
        @statistic[responseTimeclass1](title="response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[responseTimeclass2](title="response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[responseTimeclass3](title="response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[responseTimeclass4](title="response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[responseTimeclass5](title="response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        
        @signal[extendedServiceTimeclass1](type="simtime_t");
        @signal[extendedServiceTimeclass2](type="simtime_t");
        @signal[extendedServiceTimeclass3](type="simtime_t");
        @signal[extendedServiceTimeclass4](type="simtime_t");
        @signal[extendedServiceTimeclass5](type="simtime_t");
        @statistic[extendedServiceTimeclass1](title="extended response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[extendedServiceTimeclass2](title="extended response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[extendedServiceTimeclass3](title="extended response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[extendedServiceTimeclass4](title="extended response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
        @statistic[extendedServiceTimeclass5](title="extended response time at end of service";unit=s;record=vector,mean;interpolationmode=none);
           
	gates:
        input inclass1;
        input inclass2;
        input inclass3;
        input inclass4;
        input inclass5;
        output out;
        
}