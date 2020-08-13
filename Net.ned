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
network Net
{
    submodules:
        class1: Source {
            parameters:
                @display("p=89,70");
        }
        class2: Source {
            parameters:
                @display("p=89,130");
        }
        class3: Source {
            parameters:
                @display("p=89,190");
        }
        class4: Source {
            parameters:
                @display("p=89,250");
        }
        class5: Source {
            parameters:
                @display("p=89,310");
        }
        sys: Queue {
        	parameters:
                @display("p=200,190");
        }    
        sink: Sink {
            parameters:
                @display("p=329,190");
        }
    connections:
        class1.out --> sys.inclass1;
        class2.out --> sys.inclass2;
        class3.out --> sys.inclass3;
        class4.out --> sys.inclass4;
        class5.out --> sys.inclass5;
        sys.out --> sink.in;
}